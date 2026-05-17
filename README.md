# FiboTowerDefense

Tower Defense game, built with raylib-cpp.

## Prerequisites

- CMake 3.11+
- C++17 compiler (GCC, Clang, MSVC)
- Git (for FetchContent to download dependencies)

### macOS
```bash
brew install cmake
```

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install cmake build-essential
```

### Windows
- Install [CMake](https://cmake.org/download/)
- Install [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) or MinGW

## Build

### macOS / Linux
```bash
mkdir build
cd build
cmake ..
make
```

### Windows (Visual Studio)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Windows (MinGW)
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

## Run

### macOS / Linux
```bash
./FiboTowerDefense
```

### Windows
```bash
.\Release\FiboTowerDefense.exe
```

## Project Structure

```
FiboTowerDefense/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── src/
│   └── main.cpp            # Entry point
├── include/                # Header files
└── assets/                 # Game assets (sprites, sounds, etc.)
```

## Dependencies

- **raylib** (v6.0) - Graphics & input library
- **raylib-cpp** (v6.0.0) - C++ wrapper for raylib

Both are automatically fetched and built via CMake's FetchContent.

## CI/CD

GitLab CI pipeline (`.gitlab-ci.yml`) automatically builds on:
- **Linux** (Ubuntu 24.04 Docker)

**Pipeline stages:**
1. **build:linux** - CMake configure + make on Linux
2. **test:build_validation** - Verify executable exists and is executable

**Artifacts:** Executable stored indefinitely after successful build.

**Triggers:** Runs on merge requests and main branch pushes.

## Development

### Adding Source Files

1. Create `.cpp` files in `src/`
2. Create `.hpp` headers in `include/`
3. Update `CMakeLists.txt` to include new sources:
   ```cmake
   add_executable(FiboTowerDefense
       src/main.cpp
       src/game.cpp
       src/tower.cpp
   )
   ```

### Compiler Warnings

Warnings are enabled by default. Treat warnings as errors during development:
```bash
cmake .. -DCMAKE_CXX_FLAGS="-Werror"
```

## Resources

- [raylib Documentation](https://www.raylib.com/)
- [raylib-cpp GitHub](https://github.com/robloach/raylib-cpp)
- [raylib Examples](https://github.com/raysan5/raylib/tree/master/examples)

## License

This project uses raylib (zlib/libpng license). See raylib repository for details.
