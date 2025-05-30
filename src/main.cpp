#include <raylib.h>
#include <emscripten.h>

const int screenWidth = 800;
const int screenHeight = 450;

// Main game loop function
void UpdateDrawFrame() {
    // Begin drawing
    BeginDrawing();
    
        // Clear background to a nice dark color
        ClearBackground(DARKGRAY);
        
        // Draw a simple red rectangle in the center
        DrawRectangle(screenWidth/2 - 150, screenHeight/2 - 75, 300, 150, RED);
        
        // Draw text inside the rectangle
        DrawText("Ramla engine running", screenWidth/2 - 100, screenHeight/2 - 30, 20, WHITE);
        DrawText("in raylib", screenWidth/2 - 40, screenHeight/2, 20, WHITE);
        
    EndDrawing();
}

int main() {
    // Initialize raylib
    InitWindow(screenWidth, screenHeight, "Ramla Engine - WebAssembly Demo");
    
    // Set the game to run at 60 FPS
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
    
    return 0;
} 