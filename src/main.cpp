#include "../include/tesseract_lib.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    auto App = std::make_unique<TESSERACT::Tesseract>();
    if (argc > 1 && std::string(argv[1]) == "--all") {
        App->RunAll();
    } else {
        App->Run();
    }
    return App->exit();
}
