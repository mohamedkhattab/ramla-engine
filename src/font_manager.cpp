#include <raylib.h>

struct FontManager {
  Font robotoRegular;
  Font robotoBold;
  bool fontsLoaded;
};

static FontManager fontManager = {0};

// Initialize the font manager and load all fonts
void initFonts() {
  if (fontManager.fontsLoaded) {
    return;
  }

  // Load embedded fonts with higher resolution (64px) for better quality when
  // scaled
  fontManager.robotoRegular =
      LoadFontEx("assets/fonts/Roboto-Regular.ttf", 64, 0, 95);
  fontManager.robotoBold =
      LoadFontEx("assets/fonts/Roboto-Bold.ttf", 64, 0, 95);

  fontManager.fontsLoaded = true;
}

// Clean up fonts when shutting down
void unloadFonts() {
  if (!fontManager.fontsLoaded) {
    return;
  }

  UnloadFont(fontManager.robotoRegular);
  UnloadFont(fontManager.robotoBold);

  fontManager.fontsLoaded = false;
}

// Get Roboto Regular font - returns default font if failed to load
Font getRobotoRegular() {
  if (!fontManager.fontsLoaded) {
    initFonts();
  }

  // Check if font loaded successfully (texture ID > 0 means success)
  if (fontManager.robotoRegular.texture.id > 0) {
    return fontManager.robotoRegular;
  }
  return GetFontDefault(); // Fallback to default font
}

// Get Roboto Bold font - returns default font if failed to load
Font getRobotoBold() {
  if (!fontManager.fontsLoaded) {
    initFonts();
  }

  // Check if font loaded successfully (texture ID > 0 means success)
  if (fontManager.robotoBold.texture.id > 0) {
    return fontManager.robotoBold;
  }

  return GetFontDefault(); // Fallback to default font
}