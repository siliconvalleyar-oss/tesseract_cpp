# Compilación

## Requisitos

- GCC 7+ o Clang con soporte C++17
- Make
- Tesseract OCR (`tesseract-ocr` y datos de idioma `tesseract-ocr-spa`)
- Sistema operativo: Linux

## Pasos

```bash
make
```

El binario se generará en `bin/reader_ocr`.

## Limpieza

```bash
make clean
```

## Ejecución

```bash
make run
```

o

```bash
./bin/reader_ocr
```
