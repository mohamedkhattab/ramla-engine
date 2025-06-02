#pragma once

// Lua integration - no extern "C" needed since everything is C++
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <cstdio>

// Global Lua state
lua_State* L = nullptr;

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
