#pragma once
#include <raylib.h>

// Draw text using logical coordinates - automatically scales to physical pixels
void DrawTextLogical(Font *font, const char *text, float logicalX,
                     float logicalY, int logicalFontSize, Color color) {
  float scale = getScaleFactor();
  float physicalX = roundf(logicalX * scale);
  float physicalY = roundf(logicalY * scale);
  float physicalFontSize = roundf(logicalFontSize * scale);

  if (font != nullptr) {
    DrawTextEx(*font, text, (Vector2){physicalX, physicalY}, physicalFontSize,
               0.0f, color);
  } else {
    DrawText(text, physicalX, physicalY, physicalFontSize, color);
  }
}

// Center text horizontally in logical coordinates
void DrawTextLogicalCentered(Font *font, const char *text, float logicalY,
                             int logicalFontSize, Color color) {
  float scale = getScaleFactor();
  float physicalFontSize = roundf(logicalFontSize * scale);

  // Measure text at physical size for accurate centering
  Vector2 textSize;
  if (font != nullptr) {
    textSize = MeasureTextEx(*font, text, physicalFontSize, 0.0f);
  } else {
    textSize.x = MeasureText(text, physicalFontSize);
    textSize.y = physicalFontSize;
  }

  // Center horizontally using logical screen width
  float logicalCenterX = logicalWidth / 2.0f;
  float physicalCenterX = roundf(logicalCenterX * scale);
  float physicalX = roundf(physicalCenterX - textSize.x / 2);
  float physicalY = roundf(logicalY * scale);

  if (font != nullptr) {
    DrawTextEx(*font, text, (Vector2){physicalX, physicalY}, physicalFontSize,
               0.0f, color);
  } else {
    DrawText(text, physicalX, physicalY, physicalFontSize, color);
  }
}