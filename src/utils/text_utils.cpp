#pragma once
#include <raylib.h>

// Draw text using logical coordinates - automatically scales to physical pixels
void DrawTextLogical(Font *font, const char *text, float logicalX,
                     float logicalY, int logicalFontSize, Color color) {
  // logicalX, logicalY, logicalFontSize are already for the current screen.
  // Raylib will handle DPR scaling.
  float physicalX = roundf(logicalX);
  float physicalY = roundf(logicalY);
  float physicalFontSize = roundf(logicalFontSize);

  if (font != nullptr) {
    DrawTextEx(*font, text, (Vector2){physicalX, physicalY}, physicalFontSize,
               0.0f, color);
  } else {
    DrawText(text, physicalX, physicalY, physicalFontSize, color);
  }
}

// Center text horizontally in logical coordinates
void DrawTextLogicalCentered(Font* font, const char* text, float logicalY_points, 
                           int logicalFontSize_points, Color color) {
    float scale = getScaleFactor(); // Needed to scale Y and Font Size from points to current logical
    
    float currentScreenLogicalFontSize = roundf(logicalFontSize_points * scale);
    float currentScreenLogicalY = roundf(logicalY_points * scale);
    
    // Measure text using its size on the current logical screen
    Vector2 textSize;
    if (font != nullptr) {
        textSize = MeasureTextEx(*font, text, currentScreenLogicalFontSize, 0.0f);
    } else {
        textSize.x = MeasureText(text, currentScreenLogicalFontSize);
        textSize.y = currentScreenLogicalFontSize;
    }
    
    // Center X on the current logical screen width
    // logicalWidth is a global from main.cpp representing the current logical viewport width
    float currentScreenLogicalCenterX = logicalWidth / 2.0f;
    float currentScreenLogicalTextX = roundf(currentScreenLogicalCenterX - textSize.x / 2.0f);
    
    if (font != nullptr) {
        DrawTextEx(*font, text, (Vector2){currentScreenLogicalTextX, currentScreenLogicalY}, currentScreenLogicalFontSize, 0.0f, color);
    } else {
        DrawText(text, currentScreenLogicalTextX, currentScreenLogicalY, currentScreenLogicalFontSize, color);
    }
}