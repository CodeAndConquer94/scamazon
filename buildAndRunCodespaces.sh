#!/bin/bash

# Exit immediately if anything fails
set -e

echo "Bootstrapping Scamazon environment..."

# Helper function to check if a command exists
check_command() {
    if ! command -v "$1" >/dev/null 2>&1; then
        echo "Missing required command: $1"
        MISSING_PACKAGES+=("$2")
    else
        echo "Found $1"
    fi
}

# Step 1: Check for required commands
MISSING_PACKAGES=()

echo "Checking required tools..."
check_command g++ g++
check_command make make
check_command pkg-config pkg-config
check_command sqlite3 sqlite3
check_command openssl openssl

# Step 2: Install missing packages if any
if [ ${#MISSING_PACKAGES[@]} -ne 0 ]; then
    echo "Installing missing packages: ${MISSING_PACKAGES[*]}"
    sudo apt update
    sudo apt install -y "${MISSING_PACKAGES[@]}" libsqlite3-dev libssl-dev
else
    echo "All required packages are already installed."
fi

# Step 3: Compile the project
echo "Locating and compiling all .cpp files..."
mkdir -p build
g++ -std=c++17 -Wall -Wextra -o build/scamazon $(find . -name "*.cpp") -lsqlite3 -lssl -lcrypto

# Step 4: Run the binary
echo "Launching Scamazon..."
./build/scamazon
