#include <raylib.h>

// Primary color palette - main brand colors
namespace Colors {
  namespace Primary {
    constexpr Color Steel = {70, 130, 180, 255};      // Main brand color
    constexpr Color SteelLight = {100, 149, 237, 255}; // Lighter variant
    constexpr Color SteelDark = {45, 100, 145, 255};   // Darker variant
    constexpr Color White = {255, 255, 255, 255};
    constexpr Color Black = {0, 0, 0, 255};
  }

  // Button color schemes
  namespace Button {
    constexpr Color Default = Primary::Steel;
    constexpr Color DefaultHover = Primary::SteelLight;
    constexpr Color DefaultPressed = Primary::SteelDark;
    
    // Success button (green tones)
    constexpr Color Success = {76, 175, 80, 255};
    constexpr Color SuccessHover = {102, 187, 106, 255};
    constexpr Color SuccessPressed = {56, 142, 60, 255};
    
    // Warning button (orange tones)
    constexpr Color Warning = {255, 152, 0, 255};
    constexpr Color WarningHover = {255, 183, 77, 255};
    constexpr Color WarningPressed = {239, 108, 0, 255};
    
    // Danger button (red tones)
    constexpr Color Danger = {244, 67, 54, 255};
    constexpr Color DangerHover = {239, 83, 80, 255};
    constexpr Color DangerPressed = {198, 40, 40, 255};
  }

  // Text colors
  namespace Text {
    constexpr Color Primary = {33, 33, 33, 255};       // Dark gray for main text
    constexpr Color Secondary = {117, 117, 117, 255};  // Medium gray for secondary text
    constexpr Color Light = {189, 189, 189, 255};      // Light gray for disabled text
    constexpr Color OnDark = {255, 255, 255, 255};     // White text for dark backgrounds
    constexpr Color Link = Primary::Steel;              // Link color matches brand
  }

  // Background colors
  namespace Background {
    constexpr Color Light = {250, 250, 250, 255};      // Very light gray
    constexpr Color Medium = {245, 245, 245, 255};     // Light gray
    constexpr Color Dark = {33, 33, 33, 255};          // Dark gray
    constexpr Color Surface = {255, 255, 255, 255};    // White for cards/surfaces
    constexpr Color Overlay = {0, 0, 0, 128};          // Semi-transparent overlay
  }

  // Status colors
  namespace Status {
    constexpr Color Success = {76, 175, 80, 255};
    constexpr Color Warning = {255, 193, 7, 255};
    constexpr Color Error = {244, 67, 54, 255};
    constexpr Color Info = Primary::Steel;
  }

  // Neutral grays (useful for borders, dividers, etc.)
  namespace Gray {
    constexpr Color Gray50 = {250, 250, 250, 255};
    constexpr Color Gray100 = {245, 245, 245, 255};
    constexpr Color Gray200 = {238, 238, 238, 255};
    constexpr Color Gray300 = {224, 224, 224, 255};
    constexpr Color Gray400 = {189, 189, 189, 255};
    constexpr Color Gray500 = {158, 158, 158, 255};
    constexpr Color Gray600 = {117, 117, 117, 255};
    constexpr Color Gray700 = {97, 97, 97, 255};
    constexpr Color Gray800 = {66, 66, 66, 255};
    constexpr Color Gray900 = {33, 33, 33, 255};
  }
}
