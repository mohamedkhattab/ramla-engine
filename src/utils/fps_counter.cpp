#include "colors.cpp"
#include <cstdio>
#include <raylib.h>

// Draw FPS counter in the top right corner
void drawFpsCounter(int screenWidth, int screenHeight) {
  // Get current FPS
  int fps = GetFPS();

  // Format the FPS text
  char fpsText[32];
  sprintf(fpsText, "FPS: %d", fps);

  // Font settings
  int fontSize = 20;
  float padding = 10.0f;

  // Measure text to position it properly
  int textWidth = MeasureText(fpsText, fontSize);

  // Position in top right corner with padding
  float textX = screenWidth - textWidth - padding;
  float textY = padding;

  // Draw the FPS counter in green
  DrawText(fpsText, textX, textY, fontSize, Colors::Status::Success);
}

// Alternative version with custom font support and scaling
void drawFpsCounterEx(int screenWidth, int screenHeight, Font *font = nullptr,
                      float scale = 1.0f) {
  // Get current FPS
  int fps = GetFPS();

  // Format the FPS text
  char fpsText[32];
  sprintf(fpsText, "FPS: %d", fps);

  // Font settings - scale the font size
  float fontSize = 20.0f * scale;
  float padding = 10.0f * scale;
  float spacing = 0.0f;

  // Measure text to position it properly
  Vector2 textSize;
  if (font != nullptr) {
    textSize = MeasureTextEx(*font, fpsText, fontSize, spacing);
  } else {
    textSize.x = MeasureText(fpsText, fontSize);
    textSize.y = fontSize;
  }

  // Position in top right corner with padding
  float textX = screenWidth - textSize.x - padding;
  float textY = padding;

  // Draw the FPS counter in green
  if (font != nullptr) {
    DrawTextEx(*font, fpsText, (Vector2){textX, textY}, fontSize, spacing,
               GREEN);
  } else {
    DrawText(fpsText, textX, textY, fontSize, GREEN);
  }
}
