#include "../include/tesseract_lib.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cctype>
#include <limits>

namespace fs = std::filesystem;

namespace TESSERACT {
    Tesseract::Tesseract()
        : inputDir("input"), outputDir("output"), lang("spa") {
        fs::create_directories(inputDir);
        fs::create_directories(outputDir);
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
            std::ifstream file(outputPath);
            if (file.is_open()) {
                std::string line;
                std::cout << "----------------------------------------\n";
                std::cout << "TEXTO EXTRAIDO:\n";
                std::cout << "----------------------------------------\n";
                while (std::getline(file, line)) {
                    std::cout << line << "\n";
                }
                std::cout << "----------------------------------------\n";
                std::cout << "Texto guardado en: " << outputPath << "\n";
                file.close();
            } else {
                std::cout << "Error: No se pudo abrir el archivo de salida.\n";
            }
        } else {
            std::cout << "Error al ejecutar Tesseract. Verifique que este instalado.\n";
            std::cout << "Instalacion: sudo apt install tesseract-ocr tesseract-ocr-spa\n";
        }
    }

    void Tesseract::showMenu() const {
        std::cout << "\n========================================\n";
        std::cout << "   LECTOR DE IMAGENES - TESSERACT OCR   \n";
        std::cout << "========================================\n";
        std::cout << "1. Procesar imagen desde input/\n";
        std::cout << "2. Ver imagenes en input/\n";
        std::cout << "3. Salir\n";
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
                std::cout << "Saliendo...\n";
                break;
            } else {
                std::cout << "Opcion invalida. Intente nuevamente.\n";
            }
        }
    }

    int Tesseract::exit() {
        return 0;
    }
}
