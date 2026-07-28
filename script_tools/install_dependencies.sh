#!/bin/bash

set -e

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "Instalando dependencias del proyecto reader_img_to_txt_tesseract..."
echo

echo "[1/3] Actualizando indices de paquetes..."
sudo apt update -qq

echo "[2/3] Instalando dependencias de C++ (compilador, make, filesystem C++17)..."
sudo apt install -y -qq g++ make

echo "[3/3] Instalando Tesseract OCR y datos de idioma..."
sudo apt install -y -qq tesseract-ocr tesseract-ocr-spa

echo
echo "Dependencias opcionales (copiar al portapapeles):"
echo "  - X11: sudo apt install xclip"
echo "  - Wayland: sudo apt install wl-clipboard"
echo

echo "Verificando instalaciones..."
if command -v tesseract &> /dev/null; then
    echo "  [OK] tesseract: $(tesseract --version | head -n1)"
else
    echo "  [FAIL] tesseract no encontrado"
fi

if command -v g++ &> /dev/null; then
    echo "  [OK] g++: $(g++ --version | head -n1)"
else
    echo "  [FAIL] g++ no encontrado"
fi

if command -v make &> /dev/null; then
    echo "  [OK] make: $(make --version | head -n1)"
else
    echo "  [FAIL] make no encontrado"
fi

echo
echo "Para compilar la app C++:"
echo "  cd $PROJECT_DIR && make"
echo
echo "Para ejecutar la app C++:"
echo "  ./reader_ocr"
echo
echo "Dependencias instaladas correctamente."
