# FiboTowerDefense

เกม Tower Defense แนววางแผน ป้องกันฐาน จากคลื่นศัตรูที่เดินจากขวาไปซ้าย

## ภาพรวม

| รายการ | รายละเอียด |
|--------|-------------|
| **ชื่อเกม** | FiboTowerDefense |
| **แนวเกม** | Tower Defense |
| **ผู้เล่น** | เดี่ยว |
| **แพลตฟอร์ม** | เดสก์ท็อป (macOS, Linux, Windows) |
| **เทคโนโลยี** | C++17, raylib v6.0, raylib-cpp v6.0.0 |

ปกป้องหอคอยของผู้เล่นจากคลื่นศัตรูที่บุกมาจากขวาใน 5 เลน วางป้อมยิง (UR3e), ป้อมประชิด (Goose), และแผงโซลาร์เซลล์บนกริด 9×5 รับทองจากการฆ่าศัตรูและจากโซลาร์เซลล์เพื่อนำมาพัฒนาป้องกัน

## วิธีการเล่น

### กฎกติกา
- ศัตรูเกิดเป็นคลื่นจากขอบขวาและเดินไปทางซ้ายเข้าหาหอคอย
- วางป้อมในช่องกริดเพื่อสกัดกั้นและทำดาเมจศัตรูก่อนถึงฐาน
- ศัตรูที่รอดไปถึงซ้ายลด HP หอคอย; HP = 0 จบเกม
- ป้อมสามารถอัปเกรด (ดาเมจ, อัตรายิง, ระยะ) และขายได้
- โซลาร์เซลล์สร้างทองแบบพาสซีฟตามเวลา
- ผ่านคลื่นไม่จำกัด — ความยากเพิ่มขึ้นเรื่อย ๆ, คะแนนเพิ่มตามจำนวนฆ่า

### ปุ่มควบคุม

| ปุ่ม | การทำงาน |
|------|----------|
| **T** | เลือกป้อมยิง (ราคา: 100 ทอง) |
| **M** | เลือกป้อมประชิด (ราคา: 80 ทอง) |
| **S** | เลือกแผงโซลาร์เซลล์ (ราคา: 60 ทอง) |
| คลิกซ้าย | วางสิ่งของที่เลือก / เลือกสิ่งของที่มีอยู่ |
| คลิกขวา | ยกเลิกโหมดก่อสร้าง |
| **P / SPACE / ESC** | หยุด / เล่นต่อ |
| **R** | เริ่มใหม่ (บนจบเกม) |
| **B,O,C,C,H,I** | เปิดโหมดโกง |

## Play

- **Windows**: [ดาวน์โหลด](https://git.bocchichan.moe/bocchi/FiboTowerDefense/-/artifacts)
- **Web**: [เล่นบนเบราว์เซอร์](https://fibotowerdefense-9feb08.p.bocchichan.moe/) (ประสิทธิภาพต่ำกว่า Windows)
- **CI**: [GitLab CI](https://git.bocchichan.moe/bocchi/FiboTowerDefense)

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

GitLab CI pipeline (`.gitlab-ci.yml`) automatically builds on push to `main`:

| Job | Platform | Artifact |
|-----|----------|----------|
| **build:windows** | MinGW-w64 cross-compile (Linux → Windows) | `.exe` + assets → `.zip` |
| **pages** | Emscripten (WebAssembly) | HTML5 build → GitLab Pages |
| **test:build_validation** | PE32 check + asset verification | — |

**Windows**: Download the `.zip` artifact from the latest pipeline.
**Web**: Play at [fibotowerdefense-9feb08.p.bocchichan.moe](https://fibotowerdefense-9feb08.p.bocchichan.moe/) (deployed on each `main` push).
**CI**: [git.bocchichan.moe/bocchi/FiboTowerDefense](https://git.bocchichan.moe/bocchi/FiboTowerDefense)

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
