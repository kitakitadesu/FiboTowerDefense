<!-- # FiboTowerDefense

เกม Tower Defense แนววางแผน ป้องกันฐาน จากคลื่นศัตรูที่เดินจากขวาไปซ้าย

## ภาพรวม

| รายการ | รายละเอียด |
|--------|-------------|
| **ชื่อเกม** | FiboTowerDefense |
| **แนวเกม** | Tower Defense |
| **ผู้เล่น** | เดี่ยว |
| **แพลตฟอร์ม** | เดสก์ท็อป (macOS, Linux, Windows) |
| **เทคโนโลยี** | C++17, raylib v6.0, raylib-cpp v6.0.0 |

ปกป้องหอคอยของคุณจากคลื่นศัตรูที่บุกมาจากขวาใน 5 เลน วางป้อมยิง (Shooting Turret), ป้อมประชิด (Melee Turret), และแผงโซลาร์เซลล์บนกริด 9×5 รับทองจากการฆ่าศัตรูและจากโซลาร์เซลล์เพื่อนำมาพัฒนาป้องกัน

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

This project uses raylib (zlib/libpng license). See raylib repository for details. -->



# 🏰 FiboTowerDefense

> เกม Tower Defense แนววางแผน ป้องกันฐาน จากคลื่นศัตรูที่เดินจากขวาไปซ้าย

| | |
|---|---|
| **Genre** | Tower Defense |
| **Player** | Single Player |
| **Platform** | Windows / macOS / Linux |
| **Technology** | C++17 · raylib v6.0 · raylib-cpp v6.0.0 |

---

## 📹 Demo Video

▶️ _[YouTube link — coming soon]_

---

## 🎮 How to Play

ศัตรูบุกมาจาก**ฝั่งขวา**ผ่าน 5 เลน มุ่งหน้าทำลายหอคอยของคุณทางซ้าย  
วางป้อมและแผงโซลาร์เซลล์บนกริด **9×5** เพื่อสกัดและกำจัดพวกมันให้หมดก่อนถึงฐาน

### 🏗️ Placeable Units

| Unit | Cost | Description |
|---|---|---|
| 🗼 **Shooting Turret** | 100g | ยิงกระสุนใส่ศัตรูในเลนเดียวกัน ระยะยิงไม่จำกัด |
| ⚔️ **Melee Turret** | 80g | โจมตีศัตรูในระยะใกล้ ดาเมจสูงกว่า |
| ☀️ **Solar Cell** | 60g | ผลิตทองแบบ passive ตามเวลา อัปเกรดได้ |

### ⚡ Game Mechanics

- **Gold** — ได้จากการกำจัดศัตรูและจาก Solar Cell; ใช้ซื้อและอัปเกรดป้อม
- **Upgrade** — เพิ่มดาเมจ, อัตรายิง, และระยะโจมตี; กดที่ป้อมเพื่อดูตัวเลือก
- **Sell** — ขายป้อมคืนได้บางส่วนเพื่อนำทองไปใช้ที่อื่น
- **Tower HP** — ศัตรูที่รอดผ่านไปได้จะลด HP; ถ้า HP = 0 แพ้ทันที
- **Enemy Waves** — มี 5 คลื่น ยากขึ้นเรื่อยๆ ทั้งพลังชีวิต ความเร็ว และจำนวน
- **Day & Night** — ทุก 10 คลื่น แผนที่จะเปลี่ยนเป็นกลางคืนพร้อมเพลงใหม่
- **Score** — เพิ่มตามจำนวนศัตรูที่กำจัด บันทึก High Score ได้หลังจบเกม

### 🎮 Controls

| Key | Action |
|---|---|
| `T` | เลือกโหมดวาง Shooting Turret |
| `M` | เลือกโหมดวาง Melee Turret |
| `S` | เลือกโหมดวาง Solar Cell |
| Left Click | วางสิ่งของ / เลือกป้อมที่มีอยู่ |
| Right Click | ยกเลิกโหมดก่อสร้าง |
| `ESC` | ปิดหน้าต่างข้อมูลป้อม / ยกเลิกโหมดก่อสร้าง |
| `P` / `Space` | Pause / Resume (มีนับถอยหลัง 3 วิก่อน resume) |
| `R` | เริ่มเกมใหม่ (หน้าจบเกม) |

---

## 🛠️ Build & Run

### Requirements

- CMake 3.11+
- C++17 compiler (GCC / Clang / MSVC)
- Git (สำหรับ FetchContent โหลด dependencies อัตโนมัติ)

**macOS**
```bash
brew install cmake
```

**Linux (Ubuntu/Debian)**
```bash
sudo apt-get install cmake build-essential
```

**Windows** — ติดตั้ง [CMake](https://cmake.org/download/) และ [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/)

---

### Build

**macOS / Linux**
```bash
mkdir build && cd build
cmake ..
make
```

**Windows (Visual Studio)**
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

**Windows (MinGW)**
```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

### Run

**macOS / Linux**
```bash
./FiboTowerDefense
```

**Windows**
```bash
.\Release\FiboTowerDefense.exe
```

> Dependencies ทั้งหมด (raylib, raylib-cpp, EnTT, raygui) จะถูกดาวน์โหลดและ build อัตโนมัติผ่าน CMake FetchContent ไม่ต้องติดตั้งเพิ่ม

---

## 📁 Project Structure

```
FiboTowerDefense/
├── CMakeLists.txt
├── README.md
├── assets/                  # รูปภาพ, เสียง, แผนที่
├── include/                 # Header files (.hpp)
│   ├── game.hpp
│   ├── level.hpp
│   ├── board.hpp
│   ├── enemy.hpp
│   ├── turret.hpp
│   ├── solar_cell.hpp
│   ├── tower.hpp
│   ├── wave_manager.hpp
│   └── ...
└── src/                     # Source files (.cpp)
    ├── main.cpp
    ├── game.cpp             # Game state machine, เมนู, pause
    ├── level.cpp            # Gameplay logic, UI panel
    ├── board.cpp            # กริดและแผนที่
    ├── enemy.cpp            # พฤติกรรมศัตรู
    ├── turret.cpp           # ระบบป้อม
    ├── solar_cell.cpp       # ระบบโซลาร์เซลล์
    └── wave_manager.cpp     # ระบบคลื่นศัตรู
```

---

## 👥 Team

| Name | Student ID |
|---|---|
| นายโชติพิสุทธิ์ มงคลวิสุทธิ์ | 68340500020 |
| นายธนนรินท์ ใจแจ้ง | 68340500029 |
| นายนภัสกรณ์ โพธิ์หล้า | 68340500034 |
| น.ส.สุพรรณิการ์ วังกุ่ม | 68340500081 |

---

## 📚 Dependencies

| Library | Version | License |
|---|---|---|
| [raylib](https://www.raylib.com/) | 6.0 | zlib |
| [raylib-cpp](https://github.com/robloach/raylib-cpp) | 6.0.0 | zlib |
| [raygui](https://github.com/raysan5/raygui) | latest | zlib |
| [EnTT](https://github.com/skypjack/entt) | latest | MIT |