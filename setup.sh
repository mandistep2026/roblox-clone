#!/bin/bash

set -e

echo "=== Roblox Clone Setup Script ==="
echo ""

VCPKG_DIR="vcpkg"

if [ ! -d "$VCPKG_DIR" ]; then
    echo "Cloning vcpkg..."
    git clone https://github.com/Microsoft/vcpkg.git
fi

if [ ! -f "$VCPKG_DIR/vcpkg" ]; then
    echo "Bootstrapping vcpkg..."
    cd vcpkg
    ./bootstrap-vcpkg.sh
    cd ..
fi

echo ""
echo "Installing dependencies..."
./vcpkg/vcpkg install

echo ""
echo "=== Setup complete! ==="
echo ""
echo "To build the project, run:"
echo "  cmake --preset=debug"
echo "  cmake --build --preset=debug"
echo ""
