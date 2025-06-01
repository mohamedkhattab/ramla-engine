# Makefile for C++ WebAssembly project using Emscripten and Raylib

# Compiler
CXX = em++

# Source files
SRC_DIR = src
BUILD_DIR = public
SRC_FILES = $(SRC_DIR)/main.cpp

# Raylib
RAYLIB_DIR = raylib/src
RAYLIB_LIB = $(RAYLIB_DIR)/libraylib.web.a

# Output files
OUTPUT = $(BUILD_DIR)/main
WASM_OUTPUT = $(OUTPUT).wasm
JS_OUTPUT = $(OUTPUT).js

# Compiler flags
CXXFLAGS = -std=c++17 -O2 -I$(RAYLIB_DIR)
EMFLAGS = -s WASM=1 \
          -s USE_GLFW=3 \
          -s ASYNCIFY \
          -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
          -s EXPORTED_FUNCTIONS='["_main"]' \
          -s ALLOW_MEMORY_GROWTH=1 \
          -s MODULARIZE=0 \
          -s EXPORT_NAME="Module" \
          --embed-file assets/fonts

# Default target
all: $(JS_OUTPUT) compile_commands.json

# Build the WebAssembly module
$(JS_OUTPUT): $(SRC_FILES) $(RAYLIB_LIB) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(EMFLAGS) $(SRC_FILES) $(RAYLIB_LIB) -o $(OUTPUT).js

# Generate compile commands for IDE integration
compile_commands.json: $(SRC_FILES)
	@echo '[' > compile_commands.json
	@echo '  {' >> compile_commands.json
	@echo '    "directory": "$(PWD)",' >> compile_commands.json
	@echo '    "command": "$(CXX) $(CXXFLAGS) --sysroot=./emsdk/upstream/emscripten/cache/sysroot -D__EMSCRIPTEN__=1 -DPLATFORM_WEB=1 -DGRAPHICS_API_OPENGL_ES2=1 -DEMSCRIPTEN $(SRC_FILES)",' >> compile_commands.json
	@echo '    "file": "$(SRC_FILES)"' >> compile_commands.json
	@echo '  }' >> compile_commands.json
	@echo ']' >> compile_commands.json

# Build raylib if it doesn't exist
$(RAYLIB_LIB):
	cd $(RAYLIB_DIR) && make PLATFORM=PLATFORM_WEB

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -f $(BUILD_DIR)/main.js $(BUILD_DIR)/main.wasm $(BUILD_DIR)/main.html

# Clean everything including raylib
clean-all: clean
	cd $(RAYLIB_DIR) && make clean PLATFORM=PLATFORM_WEB
	rm -f compile_commands.json

# Serve the project locally (requires Python)
serve: all
	@echo "Starting local server at http://localhost:9999"
	@echo "Open your browser and navigate to http://localhost:9999"
	@echo "Press Ctrl+C to stop the server"
	cd $(BUILD_DIR) && python3 -m http.server 9999

# Alternative serve command using Node.js (if you have http-server installed)
serve-node: all
	@echo "Starting local server at http://localhost:8080"
	@echo "Open your browser and navigate to http://localhost:8080"
	@echo "Press Ctrl+C to stop the server"
	cd $(BUILD_DIR) && npx http-server -p 8080

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the WebAssembly module (default)"
	@echo "  clean      - Remove build artifacts"
	@echo "  clean-all  - Remove build artifacts and clean raylib"
	@echo "  serve      - Build and serve the project locally using Python"
	@echo "  serve-node - Build and serve the project locally using Node.js"
	@echo "  help       - Show this help message"

.PHONY: all clean clean-all serve serve-node help 