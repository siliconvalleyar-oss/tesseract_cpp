#!/bin/bash

# Script para extraer texto de imágenes con Tesseract y copiarlo al portapapeles

# --- Configuración ---
IDIOMA="spa"          # Idioma por defecto (español)
# IDIOMA="spa+eng"    # Descomenta esta línea si tienes mezcla de español/inglés

# --- Validaciones ---
if [ $# -eq 0 ]; then
    echo "❌ Uso: $0 <ruta_de_la_imagen>"
    echo "   Ejemplo: $0 mi_foto.jpg"
    exit 1
fi

IMAGEN="$1"

if [ ! -f "$IMAGEN" ]; then
    echo "❌ Error: El archivo '$IMAGEN' no existe."
    exit 1
fi

# Verificar que Tesseract está instalado
if ! command -v tesseract &> /dev/null; then
    echo "❌ Tesseract no está instalado. Instálalo con: sudo apt install tesseract-ocr tesseract-ocr-spa"
    exit 1
fi

# --- Extraer texto ---
echo "🔍 Procesando imagen: $IMAGEN"
TEXTO=$(tesseract "$IMAGEN" stdout -l "$IDIOMA" 2>/dev/null)

if [ -z "$TEXTO" ] || [[ "$TEXTO" == *"Error"* ]]; then
    echo "❌ No se pudo extraer texto. ¿La imagen tiene letras legibles?"
    exit 1
fi

# --- Mostrar en pantalla ---
echo ""
echo "════════════════════════════════════════════"
echo "📝 TEXTO EXTRAÍDO:"
echo "════════════════════════════════════════════"
echo "$TEXTO"
echo "════════════════════════════════════════════"
echo ""

# --- Copiar al portapapeles (el "imgclip" casero) ---
if command -v xclip &> /dev/null; then
    # Para X11 (la mayoría de los escritorios)
    echo "$TEXTO" | xclip -selection clipboard
    echo "✅ Texto copiado al portapapeles (X11 / xclip)"
elif command -v wl-copy &> /dev/null; then
    # Para Wayland (Gnome 40+, KDE Plasma 5.25+)
    echo "$TEXTO" | wl-copy
    echo "✅ Texto copiado al portapapeles (Wayland / wl-copy)"
else
    # Si no tiene xclip ni wl-copy, probamos a usar el comando imgclip que mencionaste
    if command -v imgclip &> /dev/null; then
        echo "ℹ️  Usando 'imgclip' como respaldo para copiar..."
        imgclip "$IMAGEN" -l "$IDIOMA"
        echo "✅ Texto copiado al portapapeles (imgclip)"
    else
        echo "⚠️  No se encontró xclip, wl-copy ni imgclip."
        echo "   Instala xclip: sudo apt install xclip"
        echo "   (El texto se muestra arriba, cópialo manualmente si quieres)"
    fi
fi

