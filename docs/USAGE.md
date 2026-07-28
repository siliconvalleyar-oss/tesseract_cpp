# Uso

## Ejecutar la aplicación

```bash
./bin/reader_ocr
```

## Menú de opciones

1. **Procesar imagen desde input/**
   - Lista las imágenes disponibles en la carpeta `input/`
   - Selecciona una imagen por número
   - El texto extraído se guarda en `output/`

2. **Ver imágenes en input/**
   - Muestra todas las imágenes disponibles para procesar

3. **Salir**

## Formatos soportados

- `.jpg` / `.jpeg`
- `.png`
- `.bmp`
- `.tif` / `.tiff`

## Idioma

El idioma por defecto es español (`spa`). Para cambiarlo, modificar la constante `lang` en `src/tesseract_lib.cpp`.
