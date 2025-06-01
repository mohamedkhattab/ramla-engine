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
};

bool isPointInsideButton(Button *btn, Vector2 point) {
  return point.x >= btn->x && point.x <= btn->x + btn->width &&
         point.y >= btn->y && point.y <= btn->y + btn->height;
}

bool isButtonHovered(Button *btn) {
  return isPointInsideButton(btn, GetMousePosition());
}

bool button(Button *btn) {
  bool hovered = isButtonHovered(btn);
  bool pressed = hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
  bool clicked = hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
  
  // Determine the current color based on state
  Color currentColor = btn->backgroundColor;
  if (pressed) {
    currentColor = btn->pressedColor;
  } else if (hovered) {
    currentColor = btn->hoverColor;
  }
  
  // Draw the button with the appropriate color
  DrawRectangle(btn->x, btn->y, btn->width, btn->height, currentColor);
  DrawText(btn->text, btn->x + 10, btn->y + 10, btn->fontSize, btn->textColor);
  
  return clicked;
}
