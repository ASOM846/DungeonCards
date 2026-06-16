# DungeonSweeper

![C++](https://img.shields.io/badge/C++-23-blue.svg)
![Raylib](https://img.shields.io/badge/raylib-game_engine-red?logo=raylib)
![CMake](https://img.shields.io/badge/CMake-build_system-064F8C?logo=cmake)

<!-- ![WASM](https://img.shields.io/badge/WebAssembly-WASM-654FF0?logo=webassembly) -->

<p align="center">
  <strong>Welcome to DungeonCards.</strong>
</p>

A minimalist, strategic card-based roguelike dungeon crawler built from scratch in C++ using the Raylib library.

## Building the project

### arch linux

```
  sudo pacman -Syu --needed git base-devel cmake ninja ccache sccache pkgconf raylib
  git clone https://github.com/ASOM846/Dungeonsweeper.git
  cd Dungeonsweeper
  cmake -S . -B build -G Ninja
  cmake --build build --parallel $(nproc)
```

Copy assets folder to your build directory and run

`./build/mineswepper`
