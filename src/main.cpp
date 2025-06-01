#include "Elements/button.cpp"
#include "font_manager.cpp"
#include "utils/colors.cpp"
#include "utils/fps_counter.cpp"
#include "utils/cursor.cpp"
#include <cstdio>
#include <emscripten.h>
#include <raylib.h>
#include <raymath.h>

#define FPS 0

// Screen dimensions - will be set by JavaScript
int screenWidth = 1920;
int screenHeight = 1080;
CursorType cursorType = CursorType::Default;

int counter = 0;

// Main game loop function
void UpdateDrawFrame() {
  // Begin drawing
  BeginDrawing();

  // Clear background to a nice dark color
  ClearBackground(BLACK);
  setCursor(cursorType);

  Font roboto = getRobotoRegular();
  
  Button btn = {
      .x = screenWidth / 2 - 100.0f,
      .y = screenHeight / 2 - 50.0f,
      .width = 200.0f,
      .height = 100.0f,
      .backgroundColor = Colors::Button::Default,
      .textColor = Colors::Text::OnDark,
      .hoverColor = Colors::Button::DefaultHover,
      .pressedColor = Colors::Button::DefaultPressed,
      .borderColor = Colors::Border::Default,
      .borderWidth = 2.0f,                         // 2px border for subtle definition
      .fontSize = 24,
      .text = "Click me",
      .borderRadius = 0.3f,  // 30% roundness for nice rounded corners
      .segments = 16,        // 16 segments for smooth curves
      .font = &roboto,       // Use Roboto font
  };

  ButtonState btnState = button(&btn);

  // Set cursor based on button state
  if (btnState.hovered) {
    cursorType = CursorType::Pointer;
  }

  if (btnState.clicked) {
    counter++;
  }

  // Use Roboto Bold for the clicked message
  Font robotoBold = getRobotoBold();
  char counterText[100];
  sprintf(counterText, "Counter: %d", counter);
  DrawTextEx(robotoBold, counterText,
             (Vector2){screenWidth / 2 - 100.0f, screenHeight / 2 - 150.0f}, 
             28, 0.0f, WHITE);

  // Draw FPS counter in top right corner
  drawFpsCounterEx(screenWidth, screenHeight, &roboto);

  EndDrawing();
}

int main() {
  // Initialize raylib - the canvas size will be handled by JavaScript
  InitWindow(screenWidth, screenHeight, "Ramla Engine");
  
  // Initialize fonts
  initFonts();

  // Set the game to run at 60 FPS
  emscripten_set_main_loop(UpdateDrawFrame, FPS, 1);
  
  // Clean up fonts (this won't actually be called in browser, but good practice)
  unloadFonts();

  return 0;
}