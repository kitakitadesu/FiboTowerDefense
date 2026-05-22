cmake_minimum_required(VERSION 3.11)
project(FiboTowerDefense
    VERSION 1.0.0
    DESCRIPTION "Tower Defense game with FIBO theme"
    LANGUAGES CXX
)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Fetch raylib
include(FetchContent)
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
)
FetchContent_MakeAvailable(raylib)

# Fetch raylib-cpp
FetchContent_Declare(raylib_cpp
    GIT_REPOSITORY https://github.com/robloach/raylib-cpp.git
    GIT_TAG v6.0.0
)
FetchContent_MakeAvailable(raylib_cpp)

# Fetch entt (header-only ECS/event library)
FetchContent_Declare(entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.13.0
)
FetchContent_MakeAvailable(entt)

# Main executable
add_executable(FiboTowerDefense
    src/main.cpp
    src/board.cpp
    src/enemy.cpp
    src/projectile.cpp
    src/raygui_impl.cpp
    src/solar_cell.cpp
    src/sprite.cpp
    src/tower.cpp
    src/turret.cpp
    src/wave_manager.cpp
    src/game.cpp
    src/level.cpp
)

# Link libraries
target_link_libraries(FiboTowerDefense
    raylib
    raylib_cpp
)

# Include directories
target_include_directories(FiboTowerDefense
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${raylib_cpp_SOURCE_DIR}/include
    ${entt_SOURCE_DIR}/src
)

# Compiler flags
if(MSVC)
    target_compile_options(FiboTowerDefense PRIVATE /W4)
else()
    target_compile_options(FiboTowerDefense PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ============================================================
# Web (Emscripten) target
# ============================================================
if(EMSCRIPTEN)
    # --preload-file @assets maps absolute src → relative assets/ in virtual FS
    # so code can open "assets/map_day_0.png" at runtime.
    set(WEB_ASSETS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    set_target_properties(FiboTowerDefense PROPERTIES
        SUFFIX ".html"
        # ALLOW_MEMORY_GROWTH for large map texture (1672×942 ~6MB RGBA)
        # ASYNCIFY needed for emscripten_sleep (raylib web backend uses it)
        LINK_FLAGS "--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/web/shell.html -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s INITIAL_MEMORY=67108864 -s MAXIMUM_MEMORY=268435456 -s ASYNCIFY --preload-file ${WEB_ASSETS}@assets"
    )
endif()

# Asset directory (for runtime reference)
set_target_properties(FiboTowerDefense PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
