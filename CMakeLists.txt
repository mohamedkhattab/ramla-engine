cmake_minimum_required(VERSION 3.16)

project(RamlaEngine LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if we're building for WebAssembly
if(EMSCRIPTEN)
    message(STATUS "Building for WebAssembly with Emscripten")
    
    # Emscripten-specific settings
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    
    # Find raylib (look for the web build)
    set(RAYLIB_LIB_PATH "${CMAKE_SOURCE_DIR}/raylib/src/libraylib.web.a")
    
    if(NOT EXISTS ${RAYLIB_LIB_PATH})
        message(FATAL_ERROR "Raylib web library not found at ${RAYLIB_LIB_PATH}. Please run 'make PLATFORM=PLATFORM_WEB' in raylib/src/ directory.")
    endif()
    
    # Create raylib target
    add_library(raylib STATIC IMPORTED)
    set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION ${RAYLIB_LIB_PATH}
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/raylib/src"
    )
    
    # Emscripten compile flags
    set(EMSCRIPTEN_COMPILE_FLAGS
        -std=c++17
        -O2
        -DPLATFORM_WEB=1
        -DGRAPHICS_API_OPENGL_ES2=1
    )
    
    # Emscripten link flags
    set(EMSCRIPTEN_LINK_FLAGS
        -s WASM=1
        -s USE_GLFW=3
        -s ASYNCIFY
        -s EXPORTED_RUNTIME_METHODS=['stringToUTF8','lengthBytesUTF8']
        -s EXPORTED_FUNCTIONS=['_main','_sayHello','_add','_greet','_malloc','_free']
        -s ALLOW_MEMORY_GROWTH=1
        -s MODULARIZE=0
        -s EXPORT_NAME="Module"
        --shell-file ${CMAKE_SOURCE_DIR}/public/index.html
    )
    
    # Convert lists to strings
    string(REPLACE ";" " " EMSCRIPTEN_COMPILE_FLAGS_STR "${EMSCRIPTEN_COMPILE_FLAGS}")
    string(REPLACE ";" " " EMSCRIPTEN_LINK_FLAGS_STR "${EMSCRIPTEN_LINK_FLAGS}")
    
    # Set compile and link flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EMSCRIPTEN_COMPILE_FLAGS_STR}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_LINK_FLAGS_STR}")
    
else()
    message(STATUS "Building for native platform")
    
    # For native builds, try to find raylib using pkg-config or find_package
    find_package(raylib QUIET)
    
    if(NOT raylib_FOUND)
        # Try to find raylib manually
        find_library(RAYLIB_LIBRARY raylib PATHS /usr/local/lib /opt/homebrew/lib)
        find_path(RAYLIB_INCLUDE_DIR raylib.h PATHS /usr/local/include /opt/homebrew/include)
        
        if(RAYLIB_LIBRARY AND RAYLIB_INCLUDE_DIR)
            add_library(raylib SHARED IMPORTED)
            set_target_properties(raylib PROPERTIES
                IMPORTED_LOCATION ${RAYLIB_LIBRARY}
                INTERFACE_INCLUDE_DIRECTORIES ${RAYLIB_INCLUDE_DIR}
            )
        else()
            message(WARNING "Raylib not found for native build. WebAssembly build will still work.")
        endif()
    endif()
    
endif()

# Source files
set(SOURCES
    src/main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
if(TARGET raylib)
    target_link_libraries(${PROJECT_NAME} raylib)
    
    if(NOT EMSCRIPTEN)
        # For native builds, also link system libraries that raylib needs
        if(APPLE)
            target_link_libraries(${PROJECT_NAME} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
        elseif(UNIX)
            target_link_libraries(${PROJECT_NAME} GL pthread dl rt X11)
        endif()
    endif()
else()
    message(WARNING "Raylib target not available")
endif()

# Set output directory
if(EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/public"
        OUTPUT_NAME "main"
    )
endif()

# Custom targets for development workflow
if(EMSCRIPTEN)
    
    # Target to build raylib for web if it doesn't exist
    add_custom_target(build_raylib_web
        COMMAND make PLATFORM=PLATFORM_WEB
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/raylib/src
        COMMENT "Building raylib for WebAssembly"
    )
    
    # Make main target depend on raylib being built
    if(NOT EXISTS ${RAYLIB_LIB_PATH})
        add_dependencies(${PROJECT_NAME} build_raylib_web)
    endif()
    
    # Target to serve the project
    add_custom_target(serve
        COMMAND python3 -m http.server 9999
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/public
        COMMENT "Starting local server at http://localhost:9999"
        DEPENDS ${PROJECT_NAME}
    )
    
    # Target to clean build artifacts
    add_custom_target(clean_web
        COMMAND ${CMAKE_COMMAND} -E remove -f main.js main.wasm main.html
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/public
        COMMENT "Cleaning WebAssembly build artifacts"
    )
    
endif()

# Print build information
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")

if(EMSCRIPTEN)
    message(STATUS "Output will be generated in: ${CMAKE_SOURCE_DIR}/public/")
    message(STATUS "Run 'cmake --build . --target serve' to build and serve")
endif() 