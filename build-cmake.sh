#!/bin/bash

# build-cmake.sh - CMake build script for WebAssembly

set -e

echo "🏗️  Building with CMake for WebAssembly..."

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

# Ensure Emscripten environment is set up
if [ ! -f "emsdk/emsdk_env.sh" ]; then
    echo "❌ Emscripten not found. Please run './setup.sh' first."
    exit 1
fi

print_status "Setting up Emscripten environment..."
source emsdk/emsdk_env.sh

# Create build directory
BUILD_DIR="build-web"
if [ -d "$BUILD_DIR" ]; then
    print_status "Cleaning existing build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

print_status "Configuring CMake for WebAssembly..."
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release

print_status "Building the project..."
cmake --build . --parallel

print_success "Build completed! Files generated in public/"

cd ..

print_status "To serve the project, run:"
echo "  cmake --build $BUILD_DIR --target serve"
echo "  or"
echo "  make serve" 