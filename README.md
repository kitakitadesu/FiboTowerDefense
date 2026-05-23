# 🏰 FiboTowerDefense

> เกม Tower Defense แนววางแผน ป้องกันฐาน จากคลื่นศัตรูที่เดินจากขวาไปซ้าย

| | |
|---|---|
| **Genre** | Tower Defense |
| **Player** | Single Player |
| **Platform** | Windows / macOS / Linux |
| **Technology** | C++17 · raylib v6.0 · raylib-cpp v6.0.0 · EnTT · raygui · Emscripten · MinGW-w64 · CMake |

---

## 📹 Demo Video

▶️ **[Demo Video](https://youtu.be/PYPrsPVuYUo)**

---

## 🚀 Quick Start

| | Link |
|---|---|
| 🪟 **Download (Windows)** | [Download from GitLab Artifacts](https://git.bocchichan.moe/bocchi/FiboTowerDefense/-/artifacts) |
| 🌐 **Play in Browser** | [fibotowerdefense-9feb08.p.bocchichan.moe](https://fibotowerdefense-9feb08.p.bocchichan.moe/) |
| 🔧 **CI / Source** | [git.bocchichan.moe/bocchi/FiboTowerDefense](https://git.bocchichan.moe/bocchi/FiboTowerDefense) |

> ⚠️ Web version อาจมี performance ต่ำกว่า Windows build แนะนำให้ดาวน์โหลด Windows version เพื่อประสบการณ์ที่ดีที่สุด

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