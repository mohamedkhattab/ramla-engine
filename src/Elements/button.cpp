#include <raylib.h>
#include <cmath>

struct Button {
  float x;
  float y;
  float width;
  float height;
  Color backgroundColor;
  Color textColor;
  Color hoverColor;
  Color pressedColor;
  Color borderColor;       // Border color
  float borderWidth;       // Border thickness (0.0f = no border)
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

// Helper function to adjust color brightness (factor < 1.0 = darker, factor > 1.0 = lighter)
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
    .clicked = state.hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
  };
  
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
  if (btn->borderWidth > 0.0f) {
    Rectangle borderRect = { 
      btn->x - btn->borderWidth, 
      btn->y - btn->borderWidth, 
      btn->width + (btn->borderWidth * 2), 
      btn->height + (btn->borderWidth * 2) 
    };
    
    if (btn->borderRadius > 0.0f) {
      // Draw rounded border
      DrawRectangleRounded(borderRect, btn->borderRadius, btn->segments, currentBorderColor);
    } else {
      // Draw regular border
      DrawRectangle(borderRect.x, borderRect.y, borderRect.width, borderRect.height, currentBorderColor);
    }
  }
  
  // Draw the main button
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
