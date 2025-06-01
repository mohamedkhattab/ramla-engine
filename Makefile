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
          -s EXPORTED_FUNCTIONS='["_main", "_setScreenDimensions", "_setLogicalDimensions"]' \
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

# Watch for changes and auto-rebuild (simple polling, no external dependencies)
watch: all
	@echo "\033[1;36m🚀 Starting development server with file watching...\033[0m"
	@echo "\033[1;32m📡 Server will be available at http://localhost:9999\033[0m"
	@echo "\033[1;33m👀 Watching for changes in src/ and assets/ directories (polling every 2 seconds)\033[0m"
	@echo "\033[1;31m⛔ Press Ctrl+C to stop\033[0m"
	@echo "\033[1;32m********************************************************************************\033[0m"
	@# Start the server in the background
	@cd $(BUILD_DIR) && python3 -m http.server 9999 > /dev/null 2>&1 & \
	SERVER_PID=$$!; \
	echo "\033[1;32m✅ Server started with PID $$SERVER_PID\033[0m"; \
	trap "echo '\033[1;31m🛑 Stopping server...\033[0m'; kill $$SERVER_PID 2>/dev/null || true; exit 0" INT TERM; \
	LAST_HASH=$$(find $(SRC_DIR) assets -type f 2>/dev/null | xargs ls -la 2>/dev/null | md5 2>/dev/null || echo ""); \
	echo "\033[1;35m📸 Initial file state captured, monitoring for changes...\033[0m"; \
	echo "\033[1;32m********************************************************************************\033[0m"; \
	while true; do \
		sleep 2; \
		CURRENT_HASH=$$(find $(SRC_DIR) assets -type f 2>/dev/null | xargs ls -la 2>/dev/null | md5 2>/dev/null || echo ""); \
		if [ "$$CURRENT_HASH" != "$$LAST_HASH" ]; then \
			echo ""; \
			echo "\033[1;32m********************************************************************************\033[0m"; \
			echo "\033[1;33m🔄 FILES CHANGED - STARTING REBUILD\033[0m"; \
			echo "\033[1;32m********************************************************************************\033[0m"; \
			echo ""; \
			echo -n "\033[1;36m🔨 Building"; \
			$(MAKE) all > /tmp/build.log 2>&1 & \
			BUILD_PID=$$!; \
			SPINNER="⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏"; \
			i=0; \
			while kill -0 $$BUILD_PID 2>/dev/null; do \
				printf "\r\033[1;36m🔨 Building %c\033[0m" $$(echo $$SPINNER | cut -c$$((i % 10 + 1))); \
				sleep 0.1; \
				i=$$((i + 1)); \
			done; \
			wait $$BUILD_PID; \
			BUILD_STATUS=$$?; \
			echo ""; \
			if [ $$BUILD_STATUS -eq 0 ]; then \
				echo "\033[1;32m✅ BUILD SUCCESSFUL!\033[0m"; \
				cat /tmp/build.log; \
			else \
				echo "\033[1;31m❌ BUILD FAILED!\033[0m"; \
				cat /tmp/build.log; \
			fi; \
			echo ""; \
			echo "\033[1;32m********************************************************************************\033[0m"; \
			echo "\033[1;35m🌐 Server still running at http://localhost:9999\033[0m"; \
			echo "\033[1;32m********************************************************************************\033[0m"; \
			echo ""; \
			LAST_HASH=$$CURRENT_HASH; \
		fi; \
	done

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
	@echo "  watch      - Build, serve, and watch for file changes (auto-rebuild)"
	@echo "  help       - Show this help message"

.PHONY: all clean clean-all serve serve-node help 