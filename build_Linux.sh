#!/usr/bin/env bash
# WSL-compatible bootstrap script for Scamazon with multi-distro support
# This script avoids complex syntax to prevent line ending issues

echo "Starting Scamazon bootstrap script..."

# Detect Linux distribution
if [ -f /etc/os-release ]; then
  . /etc/os-release
  DISTRO="$ID"
  echo "Detected distribution: $DISTRO"
else
  echo "Could not detect distribution, assuming Ubuntu"
  DISTRO="ubuntu"
fi

# Install required packages based on distribution
if [ "$DISTRO" = "ubuntu" ] || [ "$DISTRO" = "debian" ] || [ "$DISTRO" = "linuxmint" ] || [ "$DISTRO" = "pop" ]; then
  echo "Installing packages for Debian-based system..."
  sudo apt update
  sudo apt install -y build-essential cmake git pkg-config libsqlite3-dev libssl-dev sqlite3 openssl

elif [ "$DISTRO" = "fedora" ] || [ "$DISTRO" = "rhel" ] || [ "$DISTRO" = "centos" ] || [ "$DISTRO" = "rocky" ] || [ "$DISTRO" = "alma" ]; then
  echo "Installing packages for Red Hat-based system..."
  sudo dnf group install -y "C Development Tools and Libraries"
  sudo dnf install -y cmake git pkg-config sqlite-devel openssl-devel sqlite openssl

elif [ "$DISTRO" = "arch" ] || [ "$DISTRO" = "manjaro" ] || [ "$DISTRO" = "endeavouros" ]; then
  echo "Installing packages for Arch-based system..."
  sudo pacman -Syu --noconfirm base-devel cmake git pkg-config sqlite openssl

elif [ "$DISTRO" = "opensuse-leap" ] || [ "$DISTRO" = "opensuse-tumbleweed" ]; then
  echo "Installing packages for openSUSE system..."
  sudo zypper --non-interactive in -t pattern devel_C_C++ devel_basis
  sudo zypper --non-interactive in cmake git pkg-config sqlite3-devel libopenssl-devel sqlite3 openssl

elif [ "$DISTRO" = "void" ]; then
  echo "Installing packages for Void Linux..."
  sudo xbps-install -Syu base-devel cmake git pkg-config sqlite-devel openssl-devel sqlite openssl

elif [ "$DISTRO" = "alpine" ]; then
  echo "Installing packages for Alpine Linux..."
  sudo apk add build-base cmake git pkgconfig sqlite-dev openssl-dev sqlite openssl

else
  echo "Unknown distribution: $DISTRO"
  echo "You will need to install the following packages manually:"
  echo "- C++ compiler (g++ or clang++)"
  echo "- make, cmake"
  echo "- pkg-config"
  echo "- sqlite3 and development headers"
  echo "- openssl and development headers"
  echo "Press Enter to continue or Ctrl+C to abort..."
  read -r
fi

# Verify g++ is installed
which g++ > /dev/null
if [ $? -ne 0 ]; then
  echo "ERROR: g++ not found, cannot continue"
  exit 1
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build

# Find and compile all cpp files
echo "Compiling all cpp files..."
g++ -std=c++17 -Wall -Wextra -o build/scamazon $(find . -name "*.cpp") -lsqlite3 -lssl -lcrypto

# Check if compilation was successful
if [ $? -eq 0 ]; then
  echo "Compilation successful!"
  echo "Do you want to run Scamazon now? (y/n)"
  read -r response
  if [[ "$response" = "y" ]] || [[ "$response" = "Y" ]]; then
    echo "Running Scamazon..."
    ./build/scamazon
  else
    echo "Scamazon built but not run. Run later with: ./build/scamazon"
  fi
else
  echo "Compilation failed!"
  exit 1
fi

echo "Scamazon environment setup complete"
exit 0