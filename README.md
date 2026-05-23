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

## เล่น

- **Windows**: [ดาวน์โหลด](https://git.bocchichan.moe/bocchi/FiboTowerDefense/-/artifacts)
- **Web**: [เล่นบนเบราว์เซอร์](https://fibotowerdefense-9feb08.p.bocchichan.moe/) (ประสิทธิภาพต่ำกว่า Windows)
- **CI**: [GitLab CI](https://git.bocchichan.moe/bocchi/FiboTowerDefense)

## ความต้องการระบบ

- CMake 3.11+
- คอมไพเลอร์ C++17 (GCC, Clang, MSVC)
- Git (สำหรับ FetchContent ดาวน์โหลด dependencies)

### macOS
```bash
brew install cmake
```

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install cmake build-essential
```

### Windows
- ติดตั้ง [CMake](https://cmake.org/download/)
- ติดตั้ง [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) หรือ MinGW

## วิธีคอมไพล์

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

## วิธีรัน

### macOS / Linux
```bash
./FiboTowerDefense
```

### Windows
```bash
.\Release\FiboTowerDefense.exe
```

## โครงสร้างโปรเจกต์

```
FiboTowerDefense/
├── CMakeLists.txt          # ตั้งค่าคอมไพล์
├── README.md               # ไฟล์นี้
├── src/
│   └── main.cpp            # จุดเริ่มต้น
├── include/                # ไฟล์หัว
└── assets/                 # ทรัพยากรเกม (ภาพ, เสียง, ฯลฯ)
```

## Dependencies

- **raylib** (v6.0) - ไลบรารีกราฟิกและอินพุต
- **raylib-cpp** (v6.0.0) - C++ wrapper สำหรับ raylib

ทั้งสองโหลดและคอมไพล์อัตโนมัติผ่าน CMake FetchContent

## CI/CD

GitLab CI (`.gitlab-ci.yml`) คอมไพล์อัตโนมัติเมื่อ push ไปที่ `main`:

| Job | แพลตฟอร์ม | ผลลัพธ์ |
|-----|-----------|---------|
| **build:windows** | MinGW-w64 ข้ามแพลตฟอร์ม (Linux → Windows) | `.exe` + assets → `.zip` |
| **pages** | Emscripten (WebAssembly) | HTML5 build → GitLab Pages |
| **test:build_validation** | ตรวจสอบ PE32 + ตรวจสอบไฟล์ assets | — |

**Windows**: ดาวน์โหลด `.zip` artifact จาก pipeline ล่าสุด
**Web**: เล่นที่ [fibotowerdefense-9feb08.p.bocchichan.moe](https://fibotowerdefense-9feb08.p.bocchichan.moe/) (อัปเดตทุกครั้งที่ push `main`)
**CI**: [git.bocchichan.moe/bocchi/FiboTowerDefense](https://git.bocchichan.moe/bocchi/FiboTowerDefense)

## การพัฒนา

### เพิ่มไฟล์ต้นฉบับ

1. สร้างไฟล์ `.cpp` ใน `src/`
2. สร้างไฟล์หัว `.hpp` ใน `include/`
3. อัปเดต `CMakeLists.txt` เพื่อรวมไฟล์ใหม่:
   ```cmake
   add_executable(FiboTowerDefense
       src/main.cpp
       src/game.cpp
       src/tower.cpp
   )
   ```

### คำเตือนคอมไพเลอร์

เปิดใช้งานคำเตือนโดยปริยาย ใช้ `-Werror` เพื่อถือคำเตือนเป็นข้อผิดพลาด:
```bash
cmake .. -DCMAKE_CXX_FLAGS="-Werror"
```

## แหล่งอ้างอิง

- [raylib Documentation](https://www.raylib.com/)
- [raylib-cpp GitHub](https://github.com/robloach/raylib-cpp)
- [raylib Examples](https://github.com/raysan5/raylib/tree/master/examples)

## ลิขสิทธิ์

โปรเจกต์นี้ใช้ raylib (สัญญาอนุญาต zlib/libpng). ดูรายละเอียดในที่เก็บ raylib.
