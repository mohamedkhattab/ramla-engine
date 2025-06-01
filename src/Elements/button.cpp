#include <cmath>
#include <raylib.h>

struct Button {
  // All dimensions are in logical pixels - engine handles scaling automatically
  float x;      // logical x
  float y;      // logical y
  float width;  // logical width
  float height; // logical height
  Color backgroundColor;
  Color textColor;
  Color hoverColor;
  Color pressedColor;
  Color borderColor; // Border color
  float borderWidth; // Border thickness in logical pixels
  int fontSize;      // Font size in logical pixels
  const char *text;
  float borderRadius; // 0.0f = no rounding, 1.0f = fully rounded
  int segments; // Number of segments for rounded corners (16 is good default)
  Font *font;   // Pointer to font (nullptr = use default font)
};

struct ButtonState {
  bool hovered;
  bool pressed;
  bool clicked;
};

bool isPointInsideButton(Button *btn, Vector2 point) {
  // Convert logical coordinates to physical for hit testing
  float scale = getScaleFactor();
  float physicalX = roundf(btn->x * scale);
  float physicalY = roundf(btn->y * scale);
  float physicalWidth = roundf(btn->width * scale);
  float physicalHeight = roundf(btn->height * scale);

  return point.x >= physicalX && point.x <= physicalX + physicalWidth &&
         point.y >= physicalY && point.y <= physicalY + physicalHeight;
}

bool isButtonHovered(Button *btn) {
  return isPointInsideButton(btn, GetMousePosition());
}

// Helper function to adjust color brightness (factor < 1.0 = darker, factor
// > 1.0 = lighter)
Color adjustColor(Color color, float factor) {
  Color newColor = color;

  if (newColor.r * factor > 255) {
    newColor.r = 255;
  }
  if (newColor.g * factor > 255) {
    newColor.g = 255;
  }
  if (newColor.b * factor > 255) {
    newColor.b = 255;
  }

  newColor.r = (unsigned char)(newColor.r * factor);
  newColor.g = (unsigned char)(newColor.g * factor);
  newColor.b = (unsigned char)(newColor.b * factor);
  newColor.a = color.a;

  return newColor;
}

ButtonState button(Button *btn) {
  ButtonState state = {
      .hovered = isButtonHovered(btn),
      .pressed = state.hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT),
      .clicked = state.hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)};

  // Convert logical coordinates to physical coordinates
  float scale = getScaleFactor();
  float physicalX = roundf(btn->x * scale);
  float physicalY = roundf(btn->y * scale);
  float physicalWidth = roundf(btn->width * scale);
  float physicalHeight = roundf(btn->height * scale);
  float physicalBorderWidth = fmaxf(1.0f, roundf(btn->borderWidth * scale));
  float physicalFontSize = roundf(btn->fontSize * scale);

  // Determine the current color based on state
  Color currentColor = btn->backgroundColor;
  Color currentBorderColor = btn->borderColor;

  if (state.pressed) {
    currentColor = btn->pressedColor;
    // Make border darker when pressed for "inset" effect
    currentBorderColor = adjustColor(btn->pressedColor, 0.6f);
  } else if (state.hovered) {
    currentColor = btn->hoverColor;
    // Slightly darker border on hover
    currentBorderColor = adjustColor(btn->hoverColor, 0.8f);
  }

  // Draw the border first (if border width > 0)
  if (physicalBorderWidth > 0.0f) {
    Rectangle borderRect = {physicalX - physicalBorderWidth,
                            physicalY - physicalBorderWidth,
                            physicalWidth + (physicalBorderWidth * 2),
                            physicalHeight + (physicalBorderWidth * 2)};

    if (btn->borderRadius > 0.0f) {
      // Draw rounded border
      DrawRectangleRounded(borderRect, btn->borderRadius, btn->segments,
                           currentBorderColor);
    } else {
      // Draw regular border
      DrawRectangle(borderRect.x, borderRect.y, borderRect.width,
                    borderRect.height, currentBorderColor);
    }
  }

  // Draw the main button
  Rectangle buttonRect = {physicalX, physicalY, physicalWidth, physicalHeight};

  if (btn->borderRadius > 0.0f) {
    // Draw rounded rectangle
    DrawRectangleRounded(buttonRect, btn->borderRadius, btn->segments,
                         currentColor);
  } else {
    // Draw regular rectangle
    DrawRectangle(physicalX, physicalY, physicalWidth, physicalHeight,
                  currentColor);
  }

  // Calculate text positioning for center alignment
  Vector2 textSize;
  float spacing = 0.0f;

  if (btn->font != nullptr) {
    textSize = MeasureTextEx(*btn->font, btn->text, physicalFontSize, spacing);
  } else {
    textSize.x = MeasureText(btn->text, physicalFontSize);
    textSize.y = physicalFontSize;
  }

  // Round text position to whole pixels for crisp rendering
  float textX = roundf(physicalX + (physicalWidth - textSize.x) / 2);
  float textY = roundf(physicalY + (physicalHeight - textSize.y) / 2);

  // Draw text with custom font or default font
  if (btn->font != nullptr) {
    DrawTextEx(*btn->font, btn->text, (Vector2){textX, textY}, physicalFontSize,
               spacing, btn->textColor);
  } else {
    DrawText(btn->text, textX, textY, physicalFontSize, btn->textColor);
  }

  return state;
}
