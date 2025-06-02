# Ramla Engine

An immediate mode UI rendering engine built with C++ WebAssembly and Raylib. Ramla Engine provides an alternative to DOM-based rendering by mapping a custom DSL (Domain Specific Language) directly to immediate mode draw calls, delivering high-performance graphics in web browsers.

![Demo Preview](https://img.shields.io/badge/Demo-Working-brightgreen) ![Platform](https://img.shields.io/badge/Platform-WebAssembly-blue) ![Language](https://img.shields.io/badge/Language-C%2B%2B17-orange)

## Overview

Traditional web UIs rely on the DOM (Document Object Model) for rendering, which can become a performance bottleneck for complex interfaces or real-time graphics. Ramla Engine takes a different approach:

- **Immediate Mode Rendering**: Direct draw calls without intermediate DOM manipulation
- **Custom DSL**: High-level UI description language that compiles to optimized graphics commands
- **WebAssembly Performance**: Near-native performance in web browsers
- **Raylib Backend**: Proven graphics library with cross-platform support

This architecture enables fluid 60+ FPS interfaces, real-time data visualizations, games, and other graphics-intensive web applications that would struggle with traditional DOM rendering.

## Features

- **Immediate Mode UI**: Direct pixel manipulation without DOM overhead
- **WebAssembly Performance**: High-performance C++ code running in browsers
- **Embedded Lua Scripting**: High-level UI logic with Lua integration (compiled as C++ for simplicity)
- **Custom DSL Mapping**: Declarative UI syntax compiled to draw calls
- **Cross-Platform**: Runs on any modern browser supporting WebAssembly
- **Modern Tooling**: Full IDE support with clangd configuration
- **Responsive**: Hardware-accelerated rendering on desktop and mobile
- **Simplified Build**: Single C++ compiler for all components

## Quick Start

### 1. Clone the Repository
```bash
git clone <your-repo-url>
cd ramla-engine
```

### 2. Run the Setup Script
```bash
chmod +x setup.sh
./setup.sh
```

### 3. Build and Run
```bash
make serve
```

Then open http://localhost:9999 in your browser to see the immediate mode rendering demo.

## Manual Setup (Alternative)

If you prefer to set up dependencies manually:

### Prerequisites

- **Git** - For cloning repositories
- **Python 3** - For running the local server
- **Make** - For building the project

### Step 1: Install Emscripten

```bash
# Clone Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate latest version
./emsdk install latest
./emsdk activate latest

# Set up environment (run this in each terminal session)
source ./emsdk_env.sh
cd ..
```

### Step 2: Install Raylib

```bash
# Clone Raylib
git clone https://github.com/raysan5/raylib.git

# Build Raylib for WebAssembly
cd raylib/src
source ../../emsdk/emsdk_env.sh
make PLATFORM=PLATFORM_WEB
cd ../..
```

### Step 3: Build the Project

```bash
# Set up Emscripten environment
source emsdk/emsdk_env.sh

# Build the WebAssembly module
make

# Or build and serve in one command
make serve
```

## Project Structure

```
ramla-engine/
├── src/
│   └── main.cpp              # Core immediate mode rendering logic
├── public/
│   └── index.html            # WebGL canvas container
├── lua/                      # Lua scripting engine (submodule)
├── emsdk/                    # Emscripten SDK (auto-installed)
├── raylib/                   # Raylib graphics library (auto-installed)
├── .clangd                   # IDE configuration for syntax highlighting
├── compile_commands.json     # Build database for IDEs
├── Makefile                  # Build configuration
├── CMakeLists.txt            # CMake build configuration
├── setup.sh                 # Automated setup script
└── README.md                 # This file
```

## Build Commands

| Command | Description |
|---------|-------------|
| `make` | Build the WebAssembly module |
| `make clean` | Remove build artifacts |
| `make serve` | Build and serve locally (Python) |
| `make serve-node` | Build and serve locally (Node.js) |
| `make watch` | 🚀 **Development mode** - Build, serve, and auto-rebuild on file changes with animated feedback |
| `make help` | Show all available commands |

## Architecture

### Immediate Mode Rendering Pipeline

1. **Lua Scripting**: High-level UI logic and component definitions
2. **DSL Input**: High-level UI description (planned)
3. **Compilation**: DSL transforms to optimized draw commands
4. **WebAssembly Execution**: C++ rendering logic runs in browser
5. **Raylib Backend**: Hardware-accelerated graphics calls
6. **Canvas Output**: Direct pixel manipulation via WebGL

### Current Demo

The current implementation demonstrates the foundation:
- WebAssembly module initialization
- Immediate mode rectangle rendering
- Text rendering with custom positioning
- 60 FPS rendering loop

### Planned Features

- Lua-based UI scripting and component system
- Custom DSL parser and compiler
- Layout management system
- Event handling for interactive UIs
- Component abstraction layer
- Performance profiling tools

## Development

```bash
make watch
```

### IDE Setup

The project includes configuration for modern IDEs:

- **VS Code/Cursor** - Full IntelliSense with C++, Raylib, and Emscripten
- **CLion** - CMake support with proper include paths
- **Any Editor** - Uses clangd language server

### Adding Rendering Logic

1. **Add Drawing Functions**:
   ```cpp
   void DrawCustomWidget(int x, int y, int width, int height) {
       DrawRectangle(x, y, width, height, BLUE);
       DrawText("Custom Widget", x + 10, y + 10, 20, WHITE);
   }
   ```

2. **Update Main Loop**:
   ```cpp
   void UpdateDrawFrame() {
       BeginDrawing();
       ClearBackground(DARKGRAY);
       
       DrawCustomWidget(100, 100, 200, 50);
       
       EndDrawing();
   }
   ```

### Build Options

**Development Build** (faster compilation):
```bash
make CXXFLAGS="-std=c++17 -O0 -g"
```

**Production Build** (optimized):
```bash
make CXXFLAGS="-std=c++17 -O3 -DNDEBUG"
```

## Performance Characteristics

### DOM vs Immediate Mode

**Traditional DOM Rendering:**
- Layout recalculation overhead
- Style computation and cascade
- Reflow and repaint cycles
- JavaScript bridge overhead

**Ramla Engine Immediate Mode:**
- Direct pixel manipulation
- Predictable rendering costs
- No layout thrashing
- Minimal JavaScript overhead

### Benchmarks

The immediate mode approach excels in scenarios with:
- High-frequency updates (real-time data)
- Complex animations
- Custom graphics requirements
- Performance-critical applications

## Deployment

### GitHub Pages
```bash
# Build the project
make

# Copy public folder to docs/ or gh-pages branch
cp -r public/* docs/
```

### Netlify/Vercel
Simply point your deployment to the `public/` folder after building.

### Self-Hosted
```bash
# Production build
make clean && make

# Serve with any web server
cd public
python3 -m http.server 8080
```

## Troubleshooting

### Common Issues

**"Module is not defined"**
- Make sure you're serving through a web server, not opening HTML directly
- Check browser console for specific errors

**"emcc: command not found"**
```bash
source emsdk/emsdk_env.sh
```

**"raylib.h not found"**
```bash
cd raylib/src && make PLATFORM=PLATFORM_WEB
```

**Build errors**
```bash
make clean && make
```

### IDE Issues

**VS Code/Cursor IntelliSense not working:**
1. Install the clangd extension
2. Disable C/C++ extension (conflicts with clangd)
3. Restart the editor

**Missing autocomplete:**
- The `.clangd` and `compile_commands.json` files provide full IDE support
- Make sure your editor supports the Language Server Protocol

## Resources

### Immediate Mode UI Concepts
- [Dear ImGui Documentation](https://github.com/ocornut/imgui)
- [Immediate Mode GUIs (IMGUI)](https://caseymuratori.com/blog_0001)
- [Retained vs Immediate Mode](https://docs.microsoft.com/en-us/windows/win32/learnwin32/retained-mode-versus-immediate-mode)

### WebAssembly Resources
- [Emscripten Documentation](https://emscripten.org/docs/)
- [WebAssembly.org](https://webassembly.org/)
- [MDN WebAssembly Guide](https://developer.mozilla.org/en-US/docs/WebAssembly)

### Raylib Resources
- [Raylib Official Site](https://www.raylib.com/)
- [Raylib Examples](https://www.raylib.com/examples.html)
- [Raylib API Reference](https://www.raylib.com/cheatsheet/cheatsheet.html)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/immediate-mode-feature`)
3. Commit your changes (`git commit -m 'Add immediate mode feature'`)
4. Push to the branch (`git push origin feature/immediate-mode-feature`)
5. Open a Pull Request

---

**Ramla Engine** - Bringing immediate mode UI rendering to the web through WebAssembly and Raylib.