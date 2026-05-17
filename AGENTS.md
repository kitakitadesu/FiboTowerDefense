# FiboTowerDefense — Agent Guide

## Project

Tower Defense game with FIBO theme. Built with raylib-cpp (C++17).

## Stack

- **raylib v6.0** — C graphics/input/audio library
- **raylib-cpp v6.0.0** — header-only C++ wrapper
- **CMake 3.11+** — build system (FetchContent for deps)
- **C++17** — language standard

## Build Commands

```bash
cmake -B build          # configure + fetch raylib/raylib-cpp
cmake --build build     # compile
./build/FiboTowerDefense  # run (from project root for asset paths)
```

## Constraints

- **Desktop only** — macOS, Linux, Windows. No web/Emscripten.
- **C++17 required** — raylib-cpp needs C++17 minimum
- **No manual dep install** — FetchContent pulls raylib + raylib-cpp automatically. Need only cmake + compiler.

## Context Files

When working on this project, AI agents should load:
- `core/standards/code-quality.md` — modular design, clean code patterns
- `core/standards/documentation.md` — documentation standards

## Project Structure

```
FiboTowerDefense/
├── CMakeLists.txt        # build config
├── AGENTS.md             # this file
├── README.md             # user docs
├── src/                  # .cpp files
│   └── main.cpp          # entry point
├── include/              # .hpp headers
├── assets/               # sprites, sounds, etc.
├── .gitignore
└── .gitlab-ci.yml
```

## Rules & Conventions

### Code Style
- C++17 features allowed (structured bindings, `if constexpr`, `std::optional`, `std::variant`, etc.)
- Follow raylib-cpp idioms (raylib:: namespace, RAII wrappers)
- Prefer raylib-cpp C++ API over raw C raylib calls
- Use `raylib::` prefix for ambiguous symbols (e.g., `raylib::DrawText`)
- Headers in `include/`, sources in `src/`, grouped by module
- `-Wall -Wextra -Wpedantic` enabled (MSVC: `/W4`)

### Naming
- Files: `snake_case.hpp` / `snake_case.cpp`
- Classes: `PascalCase`
- Functions/methods: `camelCase`
- Constants: `kPascalCase` or `SCREAMING_SNAKE_CASE`
- Member variables: `m_camelCase`

### Architecture
- Modular design: Game, Tower, Enemy, Wave, etc. as separate translation units
- raylib types used directly (no custom math wrappers)
- Game state via simple enum/states (Menu, Playing, Paused, GameOver)
- Asset loading: path relative to `CMAKE_CURRENT_SOURCE_DIR` (project root)

### Git
- Keep history clean — rebase before PRs
- Force push only on feature branches, not shared main
- Commit format: `type: description` (no emojis), imperative mood, ≤72 chars

## Deps

| Dep | Version | URL |
|-----|---------|-----|
| raylib | 6.0 | https://github.com/raysan5/raylib |
| raylib-cpp | 6.0.0 | https://github.com/robloach/raylib-cpp |

Both auto-fetched via CMake FetchContent. No manual install needed (except cmake + compiler).

## CI

GitLab CI (`.gitlab-ci.yml`):
- **build:linux** — configure + make on Ubuntu 24.04 Docker
- **test:build_validation** — verify executable exists and is executable
- Triggers: MR + main branch
- Artifacts: executable retained 1 week

## Known Issues

- macOS OpenGL deprecation warning (non-fatal, raylib uses Metal fallback)
- `raylib::DrawText` ambiguity — use fully-qualified `raylib::DrawText`
- raylib v5.0.2 incompatible with raylib-cpp v6.0.0 (raymath.h operator conflict)
