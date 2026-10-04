# Tutku's Lite Game Engine

A lightweight low-poly tycoon game engine built on raylib and R3D.

## Build

Configure with CMake 3.22 or newer and a C/C++ compiler. The first configure
downloads the pinned R3D v0.11.0 and Assimp 6.0.3 sources into the build tree;
Python 3 is needed to embed R3D's shaders and lookup assets during compilation.
The engine reuses its bundled raylib 5.5 target.

```sh
cmake -S . -B build
cmake --build build -j
```
