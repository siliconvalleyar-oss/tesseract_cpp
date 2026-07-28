# tesseract_cpp

Herramienta de escritorio para extraer texto de imágenes usando Tesseract OCR con interfaz de menú en C++17.

## Tabla de Contenidos

- [Características](#características)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Requisitos](#requisitos)
- [Compilación](#compilación)
- [Uso](#uso)
- [Documentación](#documentación)
- [Licencia](#licencia)

## Características

- Procesamiento OCR de imágenes desde línea de comandos
- Soporte para múltiples formatos: JPG, PNG, BMP, TIFF
- Idioma configurable (por defecto: español)
- Menú interactivo para seleccionar imágenes
- Resultados guardados automáticamente en `output/`
- Binario limpio en `bin/`
- Objetos separados en `obj/`

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

## Requisitos

- GCC 7+ o Clang con soporte C++17
- Make
- Tesseract OCR (`tesseract-ocr` y datos de idioma `tesseract-ocr-spa`)
- Sistema operativo: Linux

## Compilación

```bash
make
```

El binario se generará en `bin/reader_ocr`.

## Limpieza

```bash
make clean
```

## Uso

```bash
./bin/reader_ocr
```

o

```bash
make run
```

## Documentación

- [Compilación](docs/BUILD.md)
- [Uso](docs/USAGE.md)

## Licencia

MIT License. Ver [LICENSE](LICENSE) para más detalles.
