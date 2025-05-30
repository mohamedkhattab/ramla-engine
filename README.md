# 🚀 C++ WebAssembly with Emscripten

A simple C++ WebAssembly project using Emscripten that demonstrates calling C++ functions from JavaScript and seeing output in the browser console.

## 📋 Prerequisites

Before you can build and run this project, you need to install Emscripten:

### Installing Emscripten

1. **Clone the Emscripten SDK:**
   ```bash
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ```

2. **Install and activate the latest Emscripten:**
   ```bash
   ./emsdk install latest
   ./emsdk activate latest
   ```

3. **Set up the environment (run this in each new terminal session):**
   ```bash
   source ./emsdk_env.sh
   ```

   Or add this to your shell profile (`.bashrc`, `.zshrc`, etc.) for permanent setup:
   ```bash
   echo 'source /path/to/emsdk/emsdk_env.sh' >> ~/.zshrc
   ```

4. **Verify installation:**
   ```bash
   emcc --version
   ```

## 🏗️ Project Structure

```
ramla-engine/
├── src/
│   └── main.cpp          # C++ source code
├── public/
│   └── index.html        # HTML interface
├── Makefile              # Build configuration
└── README.md            # This file
```

## 🔨 Building the Project

1. **Build the WebAssembly module:**
   ```bash
   make
   ```
   This will generate `main.js` and `main.wasm` files in the `public/` directory.

2. **Clean build artifacts:**
   ```bash
   make clean
   ```

## 🌐 Running the Project

Since WebAssembly requires a web server (due to CORS restrictions), you can use one of these methods:

### Option 1: Using Python (recommended)
```bash
make serve
```
Then open http://localhost:8000 in your browser.

### Option 2: Using Node.js
```bash
make serve-node
```
Then open http://localhost:8080 in your browser.

### Option 3: Manual server setup
```bash
cd public
python3 -m http.server 8000
# or
npx http-server -p 8080
```

## 🎮 Using the Demo

1. Open the webpage in your browser
2. **Open the browser's developer console (F12)** - this is where you'll see the C++ output!
3. Wait for the "WebAssembly module loaded successfully!" message
4. Click the buttons to call different C++ functions
5. Watch the console for messages from your C++ code

## 📝 What You'll See

The demo includes several C++ functions that output to the browser console:

- **sayHello()**: Prints a simple greeting
- **add(int, int)**: Adds two numbers and prints the result
- **greet(const char*)**: Prints a personalized greeting

All `std::cout` output from C++ will appear in the browser console with the prefix "C++ Output:".

## 🔧 Customizing the Code

### Adding New C++ Functions

1. Add your function to `src/main.cpp` with the `EMSCRIPTEN_KEEPALIVE` attribute:
   ```cpp
   extern "C" {
       EMSCRIPTEN_KEEPALIVE
       void myNewFunction() {
           std::cout << "Hello from my new function!" << std::endl;
       }
   }
   ```

2. Export it in the Makefile by adding it to `EXPORTED_FUNCTIONS`:
   ```makefile
   EMFLAGS = -s EXPORTED_FUNCTIONS='["_main", "_sayHello", "_add", "_greet", "_myNewFunction"]'
   ```

3. Call it from JavaScript:
   ```javascript
   Module._myNewFunction();
   ```

4. Rebuild:
   ```bash
   make clean && make
   ```

## 🐛 Troubleshooting

- **"Module is not defined" error**: Make sure you're serving the files through a web server, not opening the HTML file directly
- **Functions not found**: Ensure your C++ functions are marked with `EMSCRIPTEN_KEEPALIVE` and exported in the Makefile
- **No console output**: Check that you have the browser's developer console open (F12)
- **Build errors**: Verify that Emscripten is properly installed and activated (`emcc --version`)

### VS Code IntelliSense Issues

If VS Code shows `'emscripten.h' file not found` errors:

1. **Make sure Emscripten environment is set up**:
   ```bash
   source /path/to/emsdk/emsdk_env.sh
   ```

2. **Reload VS Code window**: Press `Cmd+Shift+P` (or `Ctrl+Shift+P`) and run "Developer: Reload Window"

3. **Check C++ configuration**: The project includes `.vscode/c_cpp_properties.json` with Emscripten paths. If you installed Emscripten in a different location, update the paths accordingly.

4. **Alternative**: If the environment variable approach doesn't work, you can manually update the paths in `.vscode/c_cpp_properties.json` to point to your Emscripten installation.

The code will still compile and work correctly even if VS Code shows these errors - they're just IntelliSense issues.

## 📚 Next Steps

This is a basic example to get you started. You can extend it by:

- Adding more complex C++ functionality
- Implementing data structures and algorithms
- Creating a game engine or graphics demo
- Integrating with existing C++ libraries
- Adding file I/O or networking capabilities

Happy coding! 🎉