#ifndef TESSERACT_LIB_HPP
#define TESSERACT_LIB_HPP

#include <string>
#include <vector>
#include <memory>
#include <ctime>

namespace TESSERACT {
    class Tesseract {
    public:
        Tesseract();
        void Run();
        void RunAll();
        int exit();
    private:
        std::string inputDir;
        std::string outputDir;
        std::string logDir;
        std::string lang;

        void showMenu() const;
        void listImages() const;
        void processImage();
        void processAllImages();
        std::vector<std::string> getImages() const;
        std::string getLogFilename() const;
        std::string readTextFile(const std::string& path) const;
    };
}

#endif
