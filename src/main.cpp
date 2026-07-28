#include "../include/tesseract_lib.hpp"

int main() {
    auto App = std::make_unique<TESSERACT::Tesseract>();
    App->Run();
    return App->exit();
}
