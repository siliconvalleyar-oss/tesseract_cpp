#!/bin/bash

# Verificar que se pasó un argumento (la ruta de la imagen)
if [ $# -eq 0 ]; then
    echo "Uso: $0 <ruta_de_la_imagen>"
    exit 1
fi

IMAGEN="$1"

# Verificar que el archivo existe
if [ ! -f "$IMAGEN" ]; then
    echo "Error: El archivo '$IMAGEN' no existe."
    exit 1
fi

# Nombre base para el archivo de salida (sin extensión)
BASE="${IMAGEN%.*}"

# Ejecutar Tesseract (idioma español, salida en archivo .txt)
echo "Convirtiendo '$IMAGEN' a texto con Tesseract (idioma español)..."
tesseract "$IMAGEN" "$BASE" -l spa

# Comprobar si Tesseract terminó correctamente
if [ $? -eq 0 ]; then
    echo "Conversión exitosa."
    echo "----- Texto extraído -----"
    cat "${BASE}.txt"
    echo "--------------------------"
    echo "El texto se guardó en '${BASE}.txt'"
else
    echo "Error: Tesseract falló al procesar la imagen."
    exit 1
fi


