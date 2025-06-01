#pragma once
#include <emscripten.h>

enum class CursorType {
    Default,
    Pointer,
    Text,
    Crosshair,
    Move,
    NotAllowed,
    Grab,
    Grabbing
};

// Change the canvas cursor style using JavaScript
void setCursor(CursorType cursor) {
    const char* cursorStyle;
    
    switch (cursor) {
        case CursorType::Default:
            cursorStyle = "default";
            break;
        case CursorType::Pointer:
            cursorStyle = "pointer";
            break;
        case CursorType::Text:
            cursorStyle = "text";
            break;
        case CursorType::Crosshair:
            cursorStyle = "crosshair";
            break;
        case CursorType::Move:
            cursorStyle = "move";
            break;
        case CursorType::NotAllowed:
            cursorStyle = "not-allowed";
            break;
        case CursorType::Grab:
            cursorStyle = "grab";
            break;
        case CursorType::Grabbing:
            cursorStyle = "grabbing";
            break;
    }
    
    // Execute JavaScript to change the canvas cursor
    char jsCode[256];
    snprintf(jsCode, sizeof(jsCode), 
        "document.getElementById('canvas').style.cursor = '%s';", 
        cursorStyle);
    emscripten_run_script(jsCode);
}

// Convenience functions for common cursor types
void setDefaultCursor() { setCursor(CursorType::Default); }
void setPointerCursor() { setCursor(CursorType::Pointer); }
void setTextCursor() { setCursor(CursorType::Text); } 
