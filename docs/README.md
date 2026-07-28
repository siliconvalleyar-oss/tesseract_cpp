# tesseract_cpp

Herramienta de escritorio para extraer texto de imágenes usando Tesseract OCR con interfaz de menú en C++17.

## Características

- Procesamiento OCR de imágenes desde línea de comandos
- Soporte para múltiples formatos: JPG, PNG, BMP, TIFF
- Idioma configurable (por defecto: español)
- Menú interactivo para seleccionar imágenes
- Resultados guardados automáticamente en `output/`

## Estructura del proyecto

```
.
├── bin/
│   └── reader_ocr
├── include/
│   └── tesseract_lib.hpp
├── obj/
│   └── src/
│       ├── main.o
│       └── tesseract_lib.o
├── src/
│   ├── main.cpp
│   └── tesseract_lib.cpp
├── input/
│   └── d.jpeg
├── output/
├── Makefile
├── LICENSE
└── docs/
    ├── README.md
    ├── BUILD.md
    └── USAGE.md
```
