#pragma once

// Lua integration - no extern "C" needed since everything is C++
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <cstdio>

// Global Lua state
lua_State* L = nullptr;

// Forward declaration of button function
ButtonState button(Button *btn);

// Lua binding for button function
static int lua_button(lua_State* L) {
    // Expect a table as the first argument
    if (!lua_istable(L, 1)) {
        luaL_error(L, "Expected table as first argument to button()");
        return 0;
    }
    
    // Create Button struct from Lua table
    Button btn = {};
    
    // Get required fields
    lua_getfield(L, 1, "x");
    btn.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    lua_getfield(L, 1, "y");
    btn.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    lua_getfield(L, 1, "width");
    btn.width = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    lua_getfield(L, 1, "height");
    btn.height = lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    lua_getfield(L, 1, "text");
    btn.text = lua_tostring(L, -1);
    lua_pop(L, 1);
    
    // Get optional fields with defaults
    lua_getfield(L, 1, "fontSize");
    btn.fontSize = lua_isnumber(L, -1) ? lua_tonumber(L, -1) : 56;
    lua_pop(L, 1);
    
    lua_getfield(L, 1, "borderWidth");
    btn.borderWidth = lua_isnumber(L, -1) ? lua_tonumber(L, -1) : 2.0f;
    lua_pop(L, 1);
    
    lua_getfield(L, 1, "borderRadius");
    btn.borderRadius = lua_isnumber(L, -1) ? lua_tonumber(L, -1) : 0.3f;
    lua_pop(L, 1);
    
    lua_getfield(L, 1, "segments");
    btn.segments = lua_isnumber(L, -1) ? lua_tonumber(L, -1) : 16;
    lua_pop(L, 1);
    
    // Check if useRoboto is specified
    lua_getfield(L, 1, "useRoboto");
    bool useRoboto = lua_isboolean(L, -1) ? lua_toboolean(L, -1) : true; // Default to true
    lua_pop(L, 1);
    
    // Set colors using the same colors as the original C++ button
    btn.backgroundColor = (Color){74, 144, 226, 255};   // Colors::Button::Default equivalent
    btn.textColor = (Color){255, 255, 255, 255};        // Colors::Text::OnDark equivalent  
    btn.hoverColor = (Color){94, 164, 246, 255};        // Colors::Button::DefaultHover equivalent
    btn.pressedColor = (Color){54, 124, 206, 255};      // Colors::Button::DefaultPressed equivalent
    btn.borderColor = (Color){100, 100, 100, 255};     // Colors::Border::Default equivalent
    
    // Set font - use roboto if requested
    if (useRoboto) {
        static Font roboto = getRobotoRegular(); // Store font statically
        btn.font = &roboto;
    } else {
        btn.font = nullptr;
    }
    
    // Call the C++ button function
    ButtonState state = button(&btn);
    
    // Return button state as a table
    lua_newtable(L);
    
    lua_pushboolean(L, state.hovered);
    lua_setfield(L, -2, "hovered");
    
    lua_pushboolean(L, state.pressed);
    lua_setfield(L, -2, "pressed");
    
    lua_pushboolean(L, state.clicked);
    lua_setfield(L, -2, "clicked");
    
    return 1; // Return the state table
}

// Initialize Lua
void initLua() {
    L = luaL_newstate();
    luaL_openlibs(L);
    
    // Register our C++ functions with Lua
    lua_register(L, "button", lua_button);
    
    // Test Lua is working
    const char* test_script = R"(
        function getWelcomeMessage()
            return "Hello from Lua!"
        end
        
        function multiply(a, b)
            return a * b
        end
        
        -- Example button function with roboto font and centered positioning
        function drawTestButton()
            -- Center the button on the reference 1920x1080 design
            local btnWidth = 300
            local btnHeight = 120
            local centerX = (1920 - btnWidth) / 2
            local centerY = (1080 - btnHeight) / 2
            
            local btnState = button({
                x = centerX,
                y = centerY,
                width = btnWidth,
                height = btnHeight,
                text = "Lua Button!",
                fontSize = 56,
                borderWidth = 2,
                borderRadius = 0.3,  -- 30% rounded corners
                segments = 16,
                useRoboto = true     -- Use roboto font
            })
            
            return btnState
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

// Call a Lua function and return the result as a string
const char* callLuaFunction(const char* functionName) {
    if (!L) return nullptr;
    
    lua_getglobal(L, functionName);
    if (lua_pcall(L, 0, 1, 0) == LUA_OK) {
        const char* result = lua_tostring(L, -1);
        // Note: Don't pop here, let the caller handle the stack
        return result;
    }
    return nullptr;
}

// Call a Lua math function with two numbers
double callLuaMathFunction(const char* functionName, double a, double b) {
    if (!L) return 0.0;
    
    lua_getglobal(L, functionName);
    lua_pushnumber(L, a);
    lua_pushnumber(L, b);
    if (lua_pcall(L, 2, 1, 0) == LUA_OK) {
        double result = lua_tonumber(L, -1);
        lua_pop(L, 1); // Remove result from stack
        return result;
    }
    return 0.0;
}

// Pop values from Lua stack (utility function)
void popLuaStack(int count) {
    if (L) {
        lua_pop(L, count);
    }
}

// Call a Lua UI function and return ButtonState
ButtonState callLuaButtonFunction(const char* functionName) {
    ButtonState state = {false, false, false};
    
    if (!L) return state;
    
    lua_getglobal(L, functionName);
    if (lua_pcall(L, 0, 1, 0) == LUA_OK) {
        if (lua_istable(L, -1)) {
            // Extract button state from returned table
            lua_getfield(L, -1, "hovered");
            state.hovered = lua_toboolean(L, -1);
            lua_pop(L, 1);
            
            lua_getfield(L, -1, "pressed");
            state.pressed = lua_toboolean(L, -1);
            lua_pop(L, 1);
            
            lua_getfield(L, -1, "clicked");
            state.clicked = lua_toboolean(L, -1);
            lua_pop(L, 1);
        }
        lua_pop(L, 1); // Remove result table from stack
    }
    
    return state;
}
