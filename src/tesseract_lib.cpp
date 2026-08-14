#include "../include/tesseract_lib.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <limits>

namespace fs = std::filesystem;

namespace TESSERACT {
    Tesseract::Tesseract()
        : inputDir("input"), outputDir("output"), logDir("logs"), lang("spa") {
        fs::create_directories(inputDir);
        fs::create_directories(outputDir);
        fs::create_directories(logDir);
    }

    std::vector<std::string> Tesseract::getImages() const {
        std::vector<std::string> images;
        if (!fs::exists(inputDir) || !fs::is_directory(inputDir)) {
            std::cerr << "Error: La carpeta '" << inputDir << "' no existe.\n";
            return images;
        }
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                for (auto& c : ext) c = std::tolower(c);
                if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" ||
                    ext == ".bmp" || ext == ".tif" || ext == ".tiff") {
                    images.push_back(entry.path().filename().string());
                }
            }
        }
        return images;
    }

    std::string Tesseract::readTextFile(const std::string& path) const {
        std::ifstream file(path);
        if (!file.is_open()) return "";
        std::ostringstream oss;
        std::string line;
        while (std::getline(file, line)) {
            oss << line << "\n";
        }
        file.close();
        return oss.str();
    }

    void Tesseract::listImages() const {
        auto images = getImages();
        if (images.empty()) {
            std::cout << "No hay imagenes en la carpeta input/.\n";
            return;
        }
        std::cout << "\nImagenes disponibles:\n";
        for (size_t i = 0; i < images.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << images[i] << "\n";
        }
    }

    void Tesseract::processImage() {
        auto images = getImages();
        if (images.empty()) {
            std::cout << "No hay imagenes en la carpeta input/.\n";
            return;
        }

        listImages();
        std::cout << "  0. Cancelar\n";
        std::cout << "Seleccione el numero de la imagen: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (std::cin.fail() || choice < 0 || choice > static_cast<int>(images.size())) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Seleccion invalida.\n";
            return;
        }
        if (choice == 0) {
            std::cout << "Operacion cancelada.\n";
            return;
        }

        std::string selectedImage = images[choice - 1];
        std::string baseName = selectedImage.substr(0, selectedImage.find_last_of('.'));
        std::string outputPath = outputDir + "/" + baseName + ".txt";

        std::cout << "\nProcesando: " << selectedImage << "\n";
        std::cout << "Idioma: " << lang << "\n";
        std::cout << "Guardando en: " << outputPath << "\n\n";

        std::string command = "tesseract \"" + inputDir + "/" + selectedImage +
                              "\" \"" + outputDir + "/" + baseName +
                              "\" -l " + lang + " --psm 3 2>&1";
        int result = std::system(command.c_str());

        if (result == 0) {
            std::string text = readTextFile(outputPath);
            if (!text.empty()) {
                std::cout << "----------------------------------------\n";
                std::cout << "TEXTO EXTRAIDO:\n";
                std::cout << "----------------------------------------\n";
                std::cout << text;
                std::cout << "----------------------------------------\n";
                std::cout << "Texto guardado en: " << outputPath << "\n";
            } else {
                std::cout << "Error: No se pudo abrir el archivo de salida.\n";
            }
        } else {
            std::cout << "Error al ejecutar Tesseract. Verifique que este instalado.\n";
            std::cout << "Instalacion: sudo apt install tesseract-ocr tesseract-ocr-spa\n";
        }
    }

    std::string Tesseract::getLogFilename() const {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << "log_" << std::put_time(&tm, "%y%m%d%H%M%S") << ".log";
        return oss.str();
    }

    void Tesseract::processAllImages() {
        auto images = getImages();
        if (images.empty()) {
            std::cout << "No hay imagenes en la carpeta input/.\n";
            return;
        }

        std::string logFilename = getLogFilename();
        std::string logPath = logDir + "/" + logFilename;

        std::ofstream logFile(logPath);
        if (!logFile.is_open()) {
            std::cout << "Error: No se pudo crear el archivo de log en " << logDir << "/\n";
            return;
        }

        std::cout << "\nProcesando " << images.size() << " imagenes...\n";
        std::cout << "Log: " << logPath << "\n\n";

        int processed = 0;
        int errors = 0;

        for (size_t i = 0; i < images.size(); ++i) {
            const auto& selectedImage = images[i];
            std::string baseName = selectedImage.substr(0, selectedImage.find_last_of('.'));
            std::string outputPath = outputDir + "/" + baseName + ".txt";

            std::cout << "[" << (i + 1) << "/" << images.size() << "] " << selectedImage << "\n";

            std::string command = "tesseract \"" + inputDir + "/" + selectedImage +
                                  "\" \"" + outputDir + "/" + baseName +
                                  "\" -l " + lang + " --psm 3 2>&1";
            int result = std::system(command.c_str());

            auto now = std::time(nullptr);
            auto tm = *std::localtime(&now);

            if (result == 0) {
                std::string text = readTextFile(outputPath);
                if (!text.empty()) {
                    logFile << "Archivo: " << selectedImage << "\n";
                    logFile << "Fecha: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n";
                    logFile << "Texto extraido:\n";
                    logFile << text << "\n";
                    processed++;
                } else {
                    logFile << "Archivo: " << selectedImage << "\n";
                    logFile << "Fecha: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n";
                    logFile << "Estado: ERROR - No se pudo leer el archivo de salida\n\n";
                    errors++;
                }
            } else {
                logFile << "Archivo: " << selectedImage << "\n";
                logFile << "Fecha: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n";
                logFile << "Estado: ERROR - Fallo Tesseract\n\n";
                errors++;
            }
        }

        auto endNow = std::time(nullptr);
        auto endTm = *std::localtime(&endNow);

        logFile << "========================================\n";
        logFile << "Fin del procesamiento: "
                << std::put_time(&endTm, "%Y-%m-%d %H:%M:%S") << "\n";
        logFile << "Procesadas: " << processed << " | Errores: " << errors << "\n";

        logFile.close();

        std::cout << "\n========================================\n";
        std::cout << "Procesamiento completado.\n";
        std::cout << "Procesadas: " << processed << " | Errores: " << errors << "\n";
        std::cout << "Log guardado en: " << logPath << "\n";
        std::cout << "========================================\n";
    }

    void Tesseract::showMenu() const {
        std::cout << "\n========================================\n";
        std::cout << "   LECTOR DE IMAGENES - TESSERACT OCR   \n";
        std::cout << "========================================\n";
        std::cout << "1. Procesar imagen desde input/\n";
        std::cout << "2. Ver imagenes en input/\n";
        std::cout << "3. Procesar todas las imagenes\n";
        std::cout << "4. Salir\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Seleccione una opcion: ";
    }

    void Tesseract::Run() {
        while (true) {
            showMenu();

            int option;
            std::cin >> option;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Opcion invalida. Intente nuevamente.\n";
                continue;
            }

            if (option == 1) {
                processImage();
            } else if (option == 2) {
                listImages();
            } else if (option == 3) {
                processAllImages();
            } else if (option == 4) {
                std::cout << "Saliendo...\n";
                break;
            } else {
                std::cout << "Opcion invalida. Intente nuevamente.\n";
            }
        }
    }

    void Tesseract::RunAll() {
        processAllImages();
    }

    int Tesseract::exit() {
        return 0;
    }
}