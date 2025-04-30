#!/bin/bash

# Exit immediately if anything fails
set -e

echo "Bootstrapping your Scamazon macOS environment..."

# --- Step 1: Install Homebrew if missing ---
if ! command -v brew &> /dev/null; then
    echo "Homebrew not found. Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

    # Setup Homebrew environment immediately (especially for Apple Silicon)
    if [[ -d "/opt/homebrew" ]]; then
        echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
        eval "$(/opt/homebrew/bin/brew shellenv)"
    else
        echo 'eval "$(/usr/local/bin/brew shellenv)"' >> ~/.zprofile
        eval "$(/usr/local/bin/brew shellenv)"
    fi
else
    echo "Homebrew already installed."
fi

# --- Step 2: Install Required Packages IF missing ---
install_if_missing() {
    PACKAGE=$1
    if brew list "$PACKAGE" &> /dev/null; then
        echo "$PACKAGE already installed."
    else
        echo "Installing $PACKAGE..."
        brew install "$PACKAGE"
    fi
}

echo "Checking and installing required packages if missing..."
install_if_missing gcc
install_if_missing openssl
install_if_missing sqlite3
install_if_missing pkg-config

# --- Step 3: Compile the project ---
echo "Compiling project..."

# Find OpenSSL path (important!)
OPENSSL_PATH=$(brew --prefix openssl)

mkdir -p build

g++ -std=c++17 -Wall -Wextra \
    -I"$OPENSSL_PATH/include" \
    -L"$OPENSSL_PATH/lib" \
    -o build/scamazon \
    $(find . -name "*.cpp") \
    -lsqlite3 -lssl -lcrypto

echo "Build successful."

# --- Step 4: Run the binary ---
echo "Launching Scamazon!"
./build/scamazon
