#include <raylib.h>
#include <emscripten.h>
#include <iostream> // for cout

const int screenWidth = 800;
const int screenHeight = 450;

// Main game loop function
void UpdateDrawFrame() {
    // Update
    // Add any game logic here
    
    // Draw
    BeginDrawing();
    
        ClearBackground(RAYWHITE);
        
        // Draw a simple red rectangle in the center
        DrawRectangle(screenWidth/2 - 100, screenHeight/2 - 50, 200, 100, RED);
        
        // Draw some text
        DrawText("Hello, Raylib WebAssembly!", 190, 200, 20, LIGHTGRAY);
        DrawText("Press ESC to close", 300, screenHeight - 40, 20, GRAY);
        
    EndDrawing();
}

extern "C" {
    // Keep the original functions for backward compatibility
    EMSCRIPTEN_KEEPALIVE
    void sayHello() {
        std::cout << "Hello from C++ WebAssembly with Raylib!" << std::endl;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int add(int a, int b) {
        int result = a + b;
        std::cout << "C++ calculated: " << a << " + " << b << " = " << result << std::endl;
        return result;
    }
    
    EMSCRIPTEN_KEEPALIVE
    void greet(const char* name) {
        std::cout << "Hello, " << name << "! Greetings from C++ with Raylib!" << std::endl;
    }
}

int main() {
    std::cout << "Raylib WebAssembly module loaded successfully!" << std::endl;
    
    // Initialize raylib
    InitWindow(screenWidth, screenHeight, "Raylib WebAssembly - Rectangle Demo");
    
    // Set the game to run at 60 FPS
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
    
    return 0;
} 