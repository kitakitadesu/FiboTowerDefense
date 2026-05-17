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

# Main executable
add_executable(FiboTowerDefense
    src/main.cpp
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
)

# Compiler flags
if(MSVC)
    target_compile_options(FiboTowerDefense PRIVATE /W4)
else()
    target_compile_options(FiboTowerDefense PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Asset directory (for runtime reference)
set_target_properties(FiboTowerDefense PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
