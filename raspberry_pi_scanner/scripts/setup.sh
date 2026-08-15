#!/bin/bash
# AUTEL Scanner - Setup Script for Raspberry Pi

set -e

echo "=========================================="
echo "  AUTEL Scanner - Setup"
echo "=========================================="
echo ""

# Check if running on Raspberry Pi
if ! grep -q "Raspberry Pi" /proc/cpuinfo 2>/dev/null; then
    echo "Warning: No se detecto Raspberry Pi"
    read -p "Continuar de todas formas? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Update system
echo "[1/5] Actualizando sistema..."
sudo apt update

# Install dependencies
echo "[2/5] Instalando dependencias..."
sudo apt install -y \
    build-essential \
    cmake \
    git \
    i2c-tools \
    libwiringpi-dev \
    python3-pip \
    python3-smbus

# Enable I2C and SPI
echo "[3/5] Habilitando interfaces..."
sudo raspi-config nonint do_i2c 0
sudo raspi-config nonint do_spi 0

# Add user to required groups
echo "[4/5] Configurando permisos..."
sudo usermod -aG i2c,spi $USER

# Install Python dependencies (if needed for tools)
echo "[5/5] Instalando herramientas adicionales..."
pip3 install --user pyserial 2>/dev/null || true

# Create log directory
sudo mkdir -p /var/log
sudo touch /var/log/autel_scanner.log
sudo chmod 666 /var/log/autel_scanner.log

echo ""
echo "=========================================="
echo "  Setup completado!"
echo "=========================================="
echo ""
echo "IMPORTANTE: Reinicia la Raspberry Pi para aplicar cambios"
echo "  sudo reboot"
echo ""
echo "Despues de reiniciar, compila el proyecto:"
echo "  cd raspberry_pi_scanner"
echo "  mkdir build && cd build"
echo "  cmake .."
echo "  make"
echo ""
