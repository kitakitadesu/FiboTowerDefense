# Task Context: raylib-cpp CMake Setup

Session ID: 2026-05-17-raylib-setup
Created: 2026-05-17T18:22:00Z
Status: in_progress

## Current Request
Setup raylib-cpp with CMake in FiboTowerDefense directory. Create working build system + README with build/run instructions.

## Context Files (Standards to Follow)
- `/Users/bocchichan/.config/opencode/context/core/standards/code-quality.md` (modular design, clean code)
- `/Users/bocchichan/.config/opencode/context/core/standards/documentation.md` (docs standards)
- `/Users/bocchichan/.config/opencode/context/development/principles/clean-code.md` (naming, SRP)

## Reference Files (Source Material)
- raylib-cpp CMakeLists.txt (v6.0.0, header-only wrapper)
- raylib-cpp README (features, conventions, examples)
- raylib README (build instructions, platform support)

## External Docs Fetched
- raylib-cpp: Header-only C++ wrapper for raylib. Uses FetchContent pattern for dependency management.
- CMake pattern: Modern CMake (3.11+), target-based approach, FetchContent for raylib + raylib_cpp
- Build: Desktop (macOS/Linux/Windows), no web (Emscripten) for now

## Components
1. **CMakeLists.txt** (root) - FetchContent raylib + raylib_cpp, configure build
2. **Project structure** - src/, include/, assets/ directories
3. **main.cpp** - Minimal raylib window example
4. **README.md** - Build + run instructions

## Constraints
- Header-only library (raylib-cpp) → no compilation needed for wrapper
- raylib must be fetched/linked via CMake
- Desktop build only (macOS/Linux/Windows)
- C++17 minimum (raylib-cpp requirement)

## Exit Criteria
- [ ] CMakeLists.txt created + builds successfully
- [ ] Project structure initialized (src/, include/, assets/)
- [ ] main.cpp compiles and runs (raylib window)
- [ ] README.md with clear build/run steps
