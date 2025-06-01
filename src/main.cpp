#include "Elements/button.cpp"
#include "font_manager.cpp"
#include <emscripten.h>
#include <raylib.h>

// Screen dimensions - will be set by JavaScript
int screenWidth = 1920;
int screenHeight = 1080;

bool wasButtonClicked = false;

// Main game loop function
void UpdateDrawFrame() {
  // Begin drawing
  BeginDrawing();

  // Clear background to a nice dark color
  ClearBackground(BLACK);

  Font roboto = getRobotoRegular();
  
  Button btn = {
      .x = screenWidth / 2 - 100.0f,
      .y = screenHeight / 2 - 50.0f,
      .width = 200.0f,
      .height = 100.0f,
      .backgroundColor = RED,
      .textColor = WHITE,
      .hoverColor = GREEN,
      .pressedColor = BLUE,
      .fontSize = 24,
      .text = "Click me",
      .borderRadius = 0.3f,  // 30% roundness for nice rounded corners
      .segments = 16,        // 16 segments for smooth curves
      .font = &roboto,       // Use Roboto font
  };

  if (button(&btn)) {
    wasButtonClicked = true;
  }

  if (wasButtonClicked) {
    // Use Roboto Bold for the clicked message
    Font robotoBold = getRobotoBold();
    DrawTextEx(robotoBold, "Button was clicked!", 
               (Vector2){screenWidth / 2 - 120.0f, screenHeight / 2 - 150.0f}, 
               28, 0.0f, WHITE);
  }

  EndDrawing();
}

int main() {
  // Initialize raylib - the canvas size will be handled by JavaScript
  InitWindow(screenWidth, screenHeight, "Ramla Engine");
  
  // Initialize fonts
  initFonts();

  // Set the game to run at 60 FPS
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
  
  // Clean up fonts (this won't actually be called in browser, but good practice)
  unloadFonts();

  return 0;
}