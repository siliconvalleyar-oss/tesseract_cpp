#include "scanner/obd2.hpp"
#include <cstring>
#include <chrono>
#include <thread>

namespace Scanner {

OBD2::OBD2(std::shared_ptr<Hardware::MCP2515> canInterface)
    : can_(std::move(canInterface)), initialized_(false),
      requestId_(0x7DF), responseId_(0x7E8) {}

OBD2::~OBD2() {
    cleanup();
}

bool OBD2::initialize() {
    if (!can_) return false;
    initialized_ = true;
    return true;
}

void OBD2::cleanup() {
    initialized_ = false;
}

bool OBD2::sendOBD2Request(uint8_t mode, uint8_t pid, uint8_t* response, size_t& length) {
    if (!initialized_ || !can_) return false;

    Hardware::CANMessage msg{};
    msg.id = requestId_;
    msg.extended = false;
    msg.dlc = 8;
    msg.data[0] = 0x02;  // Number of additional bytes
    msg.data[1] = mode;
    msg.data[2] = pid;
    msg.data[3] = 0x00;
    msg.data[4] = 0x00;
    msg.data[5] = 0x00;
    msg.data[6] = 0x00;
    msg.data[7] = 0x00;

    if (!can_->sendMessage(msg)) {
        return false;
    }

    return waitForResponse(response, length);
}

bool OBD2::waitForResponse(uint8_t* response, size_t& length, uint32_t timeoutMs) {
    auto start = std::chrono::steady_clock::now();
    Hardware::CANMessage msg{};

    while (true) {
        if (can_->receiveMessage(msg)) {
            if (msg.id == responseId_ || msg.id == responseId_ + 1 || msg.id == responseId_ + 2) {
                memcpy(response, msg.data, msg.dlc);
                length = msg.dlc;
                return true;
            }
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
        if (elapsed >= timeoutMs) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return false;
}

bool OBD2::requestPID(uint8_t pid, uint8_t* data, size_t length) {
    uint8_t response[8] = {0};
    size_t respLen = 0;

    if (!sendOBD2Request(0x01, pid, response, respLen)) {
        return false;
    }

    if (respLen < 3 || response[1] != 0x41 || response[2] != pid) {
        return false;
    }

    memcpy(data, &response[3], length);
    return true;
}

float OBD2::calculateMAF(uint8_t a, uint8_t b) {
    return ((a * 256.0f) + b) / 100.0f;  // g/s
}

float OBD2::calculateMAP(uint8_t a, uint8_t b) {
    (void)b;
    return a;  // kPa
}

float OBD2::calculateRPM(uint8_t a, uint8_t b) {
    return ((a * 256.0f) + b) / 4.0f;
}

float OBD2::calculateSpeed(uint8_t a) {
    return static_cast<float>(a);  // km/h
}

float OBD2::calculateThrottle(uint8_t a) {
    return (a * 100.0f) / 255.0f;  // %
}

float OBD2::calculateFuelTrim(uint8_t a) {
    return (a - 128.0f) * 100.0f / 128.0f;  // %
}

float OBD2::calculateLoad(uint8_t a) {
    return (a * 100.0f) / 255.0f;  // %
}

float OBD2::calculateTemp(uint8_t a) {
    return a - 40.0f;  // °C
}

bool OBD2::requestDTCs(std::vector<std::string>& dtcs) {
    dtcs.clear();

    uint8_t response[8] = {0};
    size_t respLen = 0;

    if (!sendOBD2Request(0x03, 0x00, response, respLen)) {
        return false;
    }

    if (respLen < 3) return false;

    uint8_t count = response[2];
    for (int i = 0; i < count && (3 + i * 2) < respLen; ++i) {
        char dtcStr[5] = {0};
        snprintf(dtcStr, sizeof(dtcStr), "%c%c%c%c",
                 ((response[3 + i * 2] >> 6) & 0x03) + 'P',
                 ((response[3 + i * 2] >> 4) & 0x03) + '0',
                 ((response[3 + i * 2] >> 2) & 0x03) + '0',
                 (response[3 + i * 2] & 0x03) + '0');
        dtcs.emplace_back(dtcStr);
    }

    return true;
}

bool OBD2::clearDTCs() {
    uint8_t response[8] = {0};
    size_t respLen = 0;
    return sendOBD2Request(0x04, 0x00, response, respLen);
}

bool OBD2::requestLiveData(std::unordered_map<std::string, PIDData>& data) {
    data.clear();

    // Request common PIDs
    uint8_t pidData[8] = {0};

    // RPM (0x0C)
    if (requestPID(0x0C, pidData, 2)) {
        PIDData pid;
        pid.name = "RPM";
        pid.unit = "rpm";
        pid.value = calculateRPM(pidData[0], pidData[1]);
        pid.min = 0;
        pid.max = 8000;
        data["rpm"] = pid;
    }

    // Speed (0x0D)
    if (requestPID(0x0D, pidData, 1)) {
        PIDData pid;
        pid.name = "Speed";
        pid.unit = "km/h";
        pid.value = calculateSpeed(pidData[0]);
        pid.min = 0;
        pid.max = 250;
        data["speed"] = pid;
    }

    // Throttle (0x11)
    if (requestPID(0x11, pidData, 1)) {
        PIDData pid;
        pid.name = "Throttle";
        pid.unit = "%";
        pid.value = calculateThrottle(pidData[0]);
        pid.min = 0;
        pid.max = 100;
        data["throttle"] = pid;
    }

    // Engine Load (0x04)
    if (requestPID(0x04, pidData, 1)) {
        PIDData pid;
        pid.name = "Engine Load";
        pid.unit = "%";
        pid.value = calculateLoad(pidData[0]);
        pid.min = 0;
        pid.max = 100;
        data["load"] = pid;
    }

    // Coolant Temp (0x05)
    if (requestPID(0x05, pidData, 1)) {
        PIDData pid;
        pid.name = "Coolant Temp";
        pid.unit = "°C";
        pid.value = calculateTemp(pidData[0]);
        pid.min = -40;
        pid.max = 215;
        data["coolant_temp"] = pid;
    }

    // MAF (0x10)
    if (requestPID(0x10, pidData, 2)) {
        PIDData pid;
        pid.name = "MAF";
        pid.unit = "g/s";
        pid.value = calculateMAF(pidData[0], pidData[1]);
        pid.min = 0;
        pid.max = 1000;
        data["maf"] = pid;
    }

    return !data.empty();
}

bool OBD2::requestFreezeFrame(uint32_t dtc, std::unordered_map<std::string, PIDData>& data) {
    data.clear();
    // Freeze frame request implementation
    uint8_t response[8] = {0};
    size_t respLen = 0;

    // Request freeze frame data for specific DTC
    if (!sendOBD2Request(0x02, 0x02, response, respLen)) {
        return false;
    }

    return true;
}

bool OBD2::requestVIN(std::string& vin) {
    vin.clear();

    uint8_t response[8] = {0};
    size_t respLen = 0;

    // Request VIN (Mode 09, PID 02)
    if (!sendOBD2Request(0x09, 0x02, response, respLen)) {
        return false;
    }

    if (respLen < 4) return false;

    // VIN is ASCII in response[3..]
    for (size_t i = 3; i < respLen && response[i] != 0; ++i) {
        vin += static_cast<char>(response[i]);
    }

    return !vin.empty();
}

} // namespace Scanner
