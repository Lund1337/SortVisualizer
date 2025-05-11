#!/bin/bash

unameOut="$(uname -s)"

echo "Detecting OS..."

case "${unameOut}" in
    Linux*)
        echo "Platform: Linux"
        cp .vscode/c_cpp_properties.linux.json .vscode/c_cpp_properties.json

        echo "Checking for package manager..."
        if command -v apt &> /dev/null; then
            echo "Detected Debian/Ubuntu. Installing GTK3 and build tools..."
            sudo apt update
            sudo apt install -y libgtk-3-dev build-essential pkg-config
        elif command -v pacman &> /dev/null; then
            echo "Detected Arch Linux. Installing GTK3 and base-devel..."
            sudo pacman -Sy --needed gtk3 base-devel pkgconf
        else
            echo "Unsupported Linux distro. Please install GTK3 and build tools manually."
        fi
        ;;
    Darwin*)
        echo "Platform: macOS"
        cp .vscode/c_cpp_properties.macos.json .vscode/c_cpp_properties.json

        if ! command -v brew &> /dev/null; then
            echo "Homebrew not found. Please install Homebrew first: https://brew.sh/"
            exit 1
        fi

        echo "Installing GTK3 and pkg-config wrapper..."
        brew install gtk+3 pkg-config gtk-mac-integration
        ;;
    *)
        echo "Unsupported OS: ${unameOut}"
        exit 1
        ;;
esac

echo "Setup complete!"
