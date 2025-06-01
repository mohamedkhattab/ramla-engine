#include <cstdio>
#include <emscripten.h>
#include <raylib.h>
#include <raymath.h>

// Include cursor types first for CursorType enum
#include "utils/cursor.cpp"

#define FPS 0

// Screen dimensions - will be set by JavaScript
int screenWidth = 800; // Physical pixels for rendering
int screenHeight = 600;
int logicalWidth = 800; // Logical pixels for UI scaling
int logicalHeight = 600;
CursorType cursorType = CursorType::Default;
int counter = 0;

// Reference resolution for design (similar to CSS reference pixel)
const float REFERENCE_WIDTH = 1920.0f;
const float REFERENCE_HEIGHT = 1080.0f;

// Calculate scaling factor based on logical resolution vs reference
float getScaleFactor() {
  // Use logical dimensions for UI scaling calculations
  float scaleX = (float)logicalWidth / REFERENCE_WIDTH;
  float scaleY = (float)logicalHeight / REFERENCE_HEIGHT;
  return fminf(scaleX, scaleY);
}

// Expose functions to JavaScript to set dimensions
extern "C" {
EMSCRIPTEN_KEEPALIVE
void setScreenDimensions(int width, int height) {
  screenWidth = width; // Physical pixels for rendering
  screenHeight = height;
}

EMSCRIPTEN_KEEPALIVE
void setLogicalDimensions(int width, int height) {
  logicalWidth = width; // Logical pixels for UI scaling
  logicalHeight = height;
}
}

// Include remaining components after global declarations
#include "Elements/button.cpp"
#include "font_manager.cpp"
#include "utils/colors.cpp"
#include "utils/fps_counter.cpp"
#include "utils/text_utils.cpp"

// Main game loop function
void UpdateDrawFrame() {
  // Begin drawing
  BeginDrawing();

  // Clear background to a nice dark color
  ClearBackground(BLACK);

  Font roboto = getRobotoRegular();

  // Create button using centered logical coordinates
  Button btn = {
      .x = (float)(logicalWidth - 200) /
           2, // Center horizontally (200px button width)
      .y = (float)(logicalHeight - 80) /
           2,       // Center vertically (80px button height)
      .width = 200, // 200px wide
      .height = 80, // 80px tall
      .backgroundColor = Colors::Button::Default,
      .textColor = Colors::Text::OnDark,
      .hoverColor = Colors::Button::DefaultHover,
      .pressedColor = Colors::Button::DefaultPressed,
      .borderColor = Colors::Border::Default,
      .borderWidth = 2, // 2px border
      .fontSize = 24,   // 24px font
      .text = "Click me",
      .borderRadius = 0.3f, // 30% roundness for nice rounded corners
      .segments = 16,       // 16 segments for smooth curves
      .font = &roboto,      // Use Roboto font
  };

  ButtonState btnState = button(&btn);

  // Set cursor based on button state
  if (btnState.hovered) {
    cursorType = CursorType::Pointer;
    setCursor(cursorType);
  } else {
    cursorType = CursorType::Default;
    setCursor(cursorType);
  }

  if (btnState.clicked) {
    counter++;
  }

  // Draw counter text above the button
  Font robotoBold = getRobotoBold();
  char counterText[100];
  sprintf(counterText, "Counter: %d", counter);
  DrawTextLogicalCentered(&robotoBold, counterText,
                          (logicalHeight - 80) / 2 - 60, 28, WHITE);

  // Draw FPS counter in top right corner
  drawFpsCounterEx(screenWidth, screenHeight, &roboto, getScaleFactor());

  EndDrawing();
}

int main() {
  // Initialize raylib - the canvas size will be handled by JavaScript
  InitWindow(screenWidth, screenHeight, "Ramla Engine");

  // Initialize fonts
  initFonts();

  // Set the game to run at 60 FPS
  emscripten_set_main_loop(UpdateDrawFrame, FPS, 1);

  // Clean up fonts (this won't actually be called in browser, but good
  // practice)
  unloadFonts();

  return 0;
}