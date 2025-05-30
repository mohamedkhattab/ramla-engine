# 🚀 C++ WebAssembly with Raylib

A complete C++ WebAssembly project using Emscripten and Raylib that renders graphics in the browser. This demo draws a simple red rectangle and demonstrates calling C++ functions from JavaScript.

![Demo Preview](https://img.shields.io/badge/Demo-Working-brightgreen) ![Platform](https://img.shields.io/badge/Platform-WebAssembly-blue) ![Language](https://img.shields.io/badge/Language-C%2B%2B17-orange)

## ✨ Features

- 🎮 **Raylib Graphics** - Hardware-accelerated 2D graphics in the browser
- 🌐 **WebAssembly** - High-performance C++ code running in web browsers
- 🔄 **Bidirectional** - Call C++ functions from JavaScript and vice versa
- 🛠️ **Modern Tooling** - Full IDE support with clangd configuration
- 📱 **Responsive** - Works on desktop and mobile browsers

## 🚀 Quick Start

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

Then open http://localhost:9999 in your browser!

## 📋 Manual Setup (Alternative)

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

## 🏗️ Project Structure

```
ramla-engine/
├── src/
│   └── main.cpp              # C++ source code with raylib graphics
├── public/
│   └── index.html            # HTML interface with WebGL canvas
├── emsdk/                    # Emscripten SDK (auto-installed)
├── raylib/                   # Raylib library (auto-installed)
├── .clangd                   # IDE configuration for syntax highlighting
├── compile_commands.json     # Build database for IDEs
├── Makefile                  # Build configuration
├── CMakeLists.txt            # CMake build configuration
├── setup.sh                 # Automated setup script
└── README.md                 # This file
```

## 🔨 Build Commands

| Command | Description |
|---------|-------------|
| `make` | Build the WebAssembly module |
| `make clean` | Remove build artifacts |
| `make serve` | Build and serve locally (Python) |
| `make serve-node` | Build and serve locally (Node.js) |
| `make help` | Show all available commands |

## 🎮 Using the Demo

1. **Open in Browser** - Navigate to http://localhost:9999
2. **Open Developer Console** (F12) - C++ output appears here
3. **See the Graphics** - Red rectangle rendered by Raylib
4. **Click Buttons** - Test JavaScript ↔ C++ communication

## 🛠️ Development

### IDE Setup

The project includes configuration for modern IDEs:

- **VS Code/Cursor** - Full IntelliSense with C++, Raylib, and Emscripten
- **CLion** - CMake support with proper include paths
- **Any Editor** - Uses clangd language server

### Adding New Features

1. **Add C++ Functions**:
   ```cpp
   extern "C" {
       EMSCRIPTEN_KEEPALIVE
       void myNewFunction() {
           std::cout << "Hello from C++!" << std::endl;
           DrawCircle(400, 225, 50, BLUE);
       }
   }
   ```

2. **Export in Makefile**:
   ```makefile
   EXPORTED_FUNCTIONS='["_main", "_sayHello", "_add", "_greet", "_myNewFunction"]'
   ```

3. **Call from JavaScript**:
   ```javascript
   Module._myNewFunction();
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

## 🌐 Deployment

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

## 🐛 Troubleshooting

### Common Issues

**❌ "Module is not defined"**
- Make sure you're serving through a web server, not opening HTML directly
- Check browser console for specific errors

**❌ "emcc: command not found"**
```bash
source emsdk/emsdk_env.sh
```

**❌ "raylib.h not found"**
```bash
cd raylib/src && make PLATFORM=PLATFORM_WEB
```

**❌ Build errors**
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

## 📚 Learn More

### Raylib Resources
- [Raylib Official Site](https://www.raylib.com/)
- [Raylib Examples](https://www.raylib.com/examples.html)
- [Raylib API Reference](https://www.raylib.com/cheatsheet/cheatsheet.html)

### WebAssembly Resources
- [Emscripten Documentation](https://emscripten.org/docs/)
- [WebAssembly.org](https://webassembly.org/)
- [MDN WebAssembly Guide](https://developer.mozilla.org/en-US/docs/WebAssembly)

### Next Steps
- 🎨 Add sprites and animations
- 🎵 Implement audio with Raylib
- 🎮 Create a simple game
- 🔧 Add more complex C++ algorithms
- 📱 Optimize for mobile devices

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

**Happy coding!** 🎉 If you build something cool with this template, please share it!