#include "Elements/button.cpp"
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

  Button btn = {
      .x = screenWidth / 2 - 100.0f,
      .y = screenHeight / 2 - 50.0f,
      .width = 200.0f,
      .height = 100.0f,
      .backgroundColor = RED,
      .textColor = WHITE,
      .hoverColor = GREEN,
      .pressedColor = BLUE,
      .fontSize = 20,
      .text = "Click me",
      .borderRadius = 0.3f,  // 30% roundness for nice rounded corners
      .segments = 16,     // 16 segments for smooth curves
  };

  if (button(&btn)) {
    wasButtonClicked = true;
  }

  if (wasButtonClicked) {
    DrawText("Button was clicked!", screenWidth / 2 - 100.0f,
             screenHeight / 2 - 150.0f, 20.0f, WHITE);
  }

  EndDrawing();
}

int main() {
  // Initialize raylib - the canvas size will be handled by JavaScript
  InitWindow(screenWidth, screenHeight, "Ramla Engine");

  // Set the game to run at 60 FPS
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);

  return 0;
}