#include <cstdio>
#include <emscripten.h>
#include <raylib.h>
#include <raymath.h>

// Lua integration - no extern "C" needed since everything is C++
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

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

// Lua state
lua_State* L = nullptr;

// Reference resolution for design (similar to CSS reference pixel)
const float REFERENCE_WIDTH = 1920.0f;
const float REFERENCE_HEIGHT = 1080.0f;

// Calculate scaling factor based on logical resolution vs reference
float getScaleFactor() {
  // Use logical dimensions for UI scaling calculations
  float scaleX = (float)logicalWidth / REFERENCE_WIDTH;
  float scaleY = (float)logicalHeight / REFERENCE_HEIGHT;
  // return fminf(scaleX, scaleY); // "Fit" or "Contain" - can make elements tiny
  return fmaxf(scaleX, scaleY); // "Cover" - makes elements larger, might crop
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

// Initialize Lua
void initLua() {
    L = luaL_newstate();
    luaL_openlibs(L);
    
    // Test Lua is working
    const char* test_script = R"(
        function getWelcomeMessage()
            return "Hello from Lua!"
        end
        
        function multiply(a, b)
            return a * b
        end
    )";
    
    int result = luaL_dostring(L, test_script);
    if (result != LUA_OK) {
        printf("Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

// Clean up Lua
void cleanupLua() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}

// Main game loop function
void UpdateDrawFrame() {
  // Begin drawing
  BeginDrawing();

  // Clear background to a nice dark color
  ClearBackground(BLACK);

  Font roboto = getRobotoRegular();

  // Create button using centered "point" coordinates (relative to 1920x1080 reference)
  Button btn = {
      .x = (REFERENCE_WIDTH - 300.0f) / 2.0f,    // Centered on 1920x1080 design
      .y = (REFERENCE_HEIGHT - 120.0f) / 2.0f,   // Centered on 1920x1080 design
      .width = 300.0f,                          // 300 points wide (larger)
      .height = 120.0f,                         // 120 points tall (larger)
      .backgroundColor = Colors::Button::Default,
      .textColor = Colors::Text::OnDark,
      .hoverColor = Colors::Button::DefaultHover,
      .pressedColor = Colors::Button::DefaultPressed,
      .borderColor = Colors::Border::Default,
      .borderWidth = 2.0f,                      // 2 points border
      .fontSize = 56,                           // 28 points font size (larger)
      .text = "Click me",
      .borderRadius = 0.3f,                     // 30% roundness
      .segments = 16,
      .font = &roboto,
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

  // Draw counter text above the button (coordinates in "points")
  Font robotoBold = getRobotoBold();
  char counterText[100];
  sprintf(counterText, "Counter: %d", counter);
  // Y position for counter: center of reference screen, minus half button height, minus some padding
  float counterTextY_points = (REFERENCE_HEIGHT - 120.0f) / 2.0f - 80.0f; 
  DrawTextLogicalCentered(&robotoBold, counterText, counterTextY_points, 56, WHITE); // 56 points font size

  // Test Lua integration - call Lua function and display result
  if (L) {
    lua_getglobal(L, "getWelcomeMessage");
    if (lua_pcall(L, 0, 1, 0) == LUA_OK) {
      const char* message = lua_tostring(L, -1);
      float luaTextY_points = counterTextY_points - 80.0f;
      DrawTextLogicalCentered(&robotoBold, message, luaTextY_points, 32, YELLOW);
      lua_pop(L, 1); // Remove result from stack
    }
    
    // Test Lua math function
    lua_getglobal(L, "multiply");
    lua_pushnumber(L, counter);
    lua_pushnumber(L, 2);
    if (lua_pcall(L, 2, 1, 0) == LUA_OK) {
      double result = lua_tonumber(L, -1);
      char mathText[100];
      sprintf(mathText, "Counter * 2 = %.0f", result);
      float mathTextY_points = counterTextY_points - 120.0f;
      DrawTextLogicalCentered(&robotoBold, mathText, mathTextY_points, 28, GREEN);
      lua_pop(L, 1); // Remove result from stack
    }
  }

  // Draw FPS counter in top right corner
  drawFpsCounterEx(screenWidth, screenHeight, &roboto);

  EndDrawing();
}

int main() {
  // Initialize raylib - the canvas size will be handled by JavaScript
  InitWindow(screenWidth, screenHeight, "Ramla Engine");

  // Initialize fonts
  initFonts();

  // Initialize Lua
  initLua();

  // Set the game to run at 60 FPS
  emscripten_set_main_loop(UpdateDrawFrame, FPS, 1);

  // Clean up fonts (this won't actually be called in browser, but good
  // practice)
  unloadFonts();
  
  // Clean up Lua
  cleanupLua();

  return 0;
}