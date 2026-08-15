#include "hardware/gpio.hpp"
#include <wiringPi.h>
#include <cerrno>
#include <cstring>
#include <functional>

namespace Hardware {

bool GPIO::initialized_ = false;

bool GPIO::initialize() {
    if (initialized_) return true;

    if (wiringPiSetup() == -1) {
        return false;
    }
    initialized_ = true;
    return true;
}

void GPIO::cleanup() {
    if (initialized_) {
        // wiringPi cleanup if needed
        initialized_ = false;
    }
}

void GPIO::setMode(uint8_t pin, PinMode mode) {
    pinMode(pin, static_cast<int>(mode));
}

void GPIO::write(uint8_t pin, bool value) {
    digitalWrite(pin, value ? HIGH : LOW);
}

bool GPIO::read(uint8_t pin) {
    return digitalRead(pin) == HIGH;
}

void GPIO::setPullUpDown(uint8_t pin, PullUpDown pud) {
    pullUpDnControl(pin, static_cast<int>(pud));
}

void GPIO::setISR(uint8_t pin, Edge edge, std::function<void()> callback) {
    int edgeMode = 0;
    switch (edge) {
        case Edge::RISING: edgeMode = INT_EDGE_RISING; break;
        case Edge::FALLING: edgeMode = INT_EDGE_FALLING; break;
        case Edge::BOTH: edgeMode = INT_EDGE_BOTH; break;
        default: edgeMode = INT_EDGE_SETUP; break;
    }

    wiringPiISR(pin, edgeMode, [](void* userdata) {
        auto* cb = static_cast<std::function<void()>*>(userdata);
        if (cb && *cb) (*cb)();
    }, new std::function<void()>(std::move(callback)));
}

} // namespace Hardware
