#include <raylib.h>

struct Button {
  float x;
  float y;
  float width;
  float height;
  Color backgroundColor;
  Color textColor;
  Color hoverColor;
  Color pressedColor;
  int fontSize;
  const char *text;
  float borderRadius;    // 0.0f = no rounding, 1.0f = fully rounded
  int segments;          // Number of segments for rounded corners (16 is good default)
  Font *font;            // Pointer to font (nullptr = use default font)
};

struct ButtonState {
  bool hovered;
  bool pressed;
  bool clicked;
};

bool isPointInsideButton(Button *btn, Vector2 point) {
  return point.x >= btn->x && point.x <= btn->x + btn->width &&
         point.y >= btn->y && point.y <= btn->y + btn->height;
}

bool isButtonHovered(Button *btn) {
  return isPointInsideButton(btn, GetMousePosition());
}

ButtonState button(Button *btn) {
  ButtonState state = {
    .hovered = isButtonHovered(btn),
    .pressed = state.hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT),
    .clicked = state.hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
  };
  
  // Determine the current color based on state
  Color currentColor = btn->backgroundColor;
  if (state.pressed) {
    currentColor = btn->pressedColor;
  } else if (state.hovered) {
    currentColor = btn->hoverColor;
  }
  
  // Draw the button with the appropriate color
  Rectangle buttonRect = { btn->x, btn->y, btn->width, btn->height };
  
  if (btn->borderRadius > 0.0f) {
    // Draw rounded rectangle
    DrawRectangleRounded(buttonRect, btn->borderRadius, btn->segments, currentColor);
  } else {
    // Draw regular rectangle
    DrawRectangle(btn->x, btn->y, btn->width, btn->height, currentColor);
  }
  
  // Calculate text positioning for center alignment
  Vector2 textSize;
  float spacing = 0.0f; // Better spacing for custom fonts
  
  if (btn->font != nullptr) {
    textSize = MeasureTextEx(*btn->font, btn->text, btn->fontSize, spacing);
  } else {
    textSize.x = MeasureText(btn->text, btn->fontSize);
    textSize.y = btn->fontSize;
  }
  
  float textX = btn->x + (btn->width - textSize.x) / 2;
  float textY = btn->y + (btn->height - textSize.y) / 2;
  
  // Draw text with custom font or default font
  if (btn->font != nullptr) {
    DrawTextEx(*btn->font, btn->text, (Vector2){textX, textY}, btn->fontSize, spacing, btn->textColor);
  } else {
    DrawText(btn->text, textX, textY, btn->fontSize, btn->textColor);
  }
  
  return state;
}
