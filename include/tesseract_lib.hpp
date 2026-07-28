#ifndef TESSERACT_LIB_HPP
#define TESSERACT_LIB_HPP

#include <string>
#include <vector>
#include <memory>

namespace TESSERACT {
    class Tesseract {
    public:
        Tesseract();
        void Run();
        int exit();
    private:
        std::string inputDir;
        std::string outputDir;
        std::string lang;

        void showMenu() const;
        void listImages() const;
        void processImage();
        std::vector<std::string> getImages() const;
    };
}

#endif
