#!/bin/bash

# setup.sh - Automated setup script for C++ WebAssembly with Raylib
# This script installs Emscripten and Raylib dependencies

set -e  # Exit on any error

echo "🚀 Setting up C++ WebAssembly with Raylib..."
echo "================================================"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if git is installed
if ! command -v git &> /dev/null; then
    print_error "Git is required but not installed. Please install git and try again."
    exit 1
fi

# Check if make is installed
if ! command -v make &> /dev/null; then
    print_error "Make is required but not installed. Please install make and try again."
    exit 1
fi

# Check if python3 is installed
if ! command -v python3 &> /dev/null; then
    print_warning "Python3 is recommended for serving the project locally."
fi

print_status "All prerequisites are available!"

# Step 1: Initialize Lua submodule
print_status "Step 1/4: Setting up Lua submodule..."

if [ ! -f "lua/lua.h" ]; then
    print_status "Initializing Lua submodule..."
    git submodule update --init --recursive
    print_success "Lua submodule initialized!"
else
    print_status "Lua submodule already available"
    print_success "Lua is ready!"
fi

# Convert Lua C files to C++ for simplified compilation
print_status "Converting Lua C files to C++ extensions..."
cd lua
for file in *.c; do
    if [ -f "$file" ]; then
        mv "$file" "${file%.c}.cpp"
    fi
done
cd ..
print_success "Lua files converted to C++ format!"

# Step 2: Install Emscripten SDK
print_status "Step 2/4: Installing Emscripten SDK..."

if [ ! -d "emsdk" ]; then
    print_status "Cloning Emscripten SDK..."
    git clone https://github.com/emscripten-core/emsdk.git
else
    print_status "Emscripten SDK already exists, updating..."
    cd emsdk
    git pull
    cd ..
fi

cd emsdk

print_status "Installing latest Emscripten..."
./emsdk install latest

print_status "Activating Emscripten..."
./emsdk activate latest

print_success "Emscripten SDK installed successfully!"
cd ..

# Step 3: Install Raylib
print_status "Step 3/4: Installing Raylib..."

if [ ! -d "raylib" ]; then
    print_status "Cloning Raylib..."
    git clone https://github.com/raysan5/raylib.git
else
    print_status "Raylib already exists, updating..."
    cd raylib
    git pull
    cd ..
fi

print_status "Building Raylib for WebAssembly..."
cd raylib/src

# Source Emscripten environment
source ../../emsdk/emsdk_env.sh

# Build raylib for web platform
make PLATFORM=PLATFORM_WEB

print_success "Raylib built successfully for WebAssembly!"
cd ../..

# Step 4: Build the project
print_status "Step 4/4: Building the project..."

# Source Emscripten environment
source emsdk/emsdk_env.sh

# Build the project
make clean || true  # Clean any existing build artifacts
make

print_success "Project built successfully!"

# Final instructions
echo ""
echo "================================================"
print_success "🎉 Setup completed successfully!"
echo ""
echo "To run the project:"
echo "  ${GREEN}make serve${NC}     # Start local server and open browser"
echo ""
echo "To rebuild:"
echo "  ${GREEN}source emsdk/emsdk_env.sh${NC}  # Set up environment"
echo "  ${GREEN}make clean && make${NC}         # Clean and rebuild"
echo ""
echo "For development:"
echo "  - Install clangd extension in your IDE for syntax highlighting"
echo "  - The project includes full IDE configuration"
echo "  - Check README.md for more details"
echo ""
print_status "Happy coding! 🚀" 