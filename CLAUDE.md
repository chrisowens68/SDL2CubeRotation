# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

A C++11 / SDL2 wireframe cube rotation demo (macOS), laid out following the `../Template` SDL2 starter. It opens a 1200x800 window and draws a rotating cube by plotting individual points with `SDL_RenderDrawPointF`, printing an FPS figure to stdout every 100 frames.

## Build & Run

```sh
make          # builds build/debug/play
make run      # build, then run (from repo root)
make clean    # remove the binary and .dSYM
```

- `all` compiles every `src/*.cpp` in one `g++` invocation (no object files / incremental builds). New `.cpp` files in `src/` are picked up automatically; subdirectories of `src/` are not.
- Flags: `-std=c++11 -Wall -O0 -g` (debug only; no release config).
- There are no tests or linters.

## Code layout

- `src/main.cpp` — main loop: `handleEvents` → `update` → `render` → `clearPoints`, plus the FPS counter.
- `src/Screen.h/.cpp` — owns the SDL window/renderer and all cube logic:
  - `setCorners` / `setConnections` define the 8 vertices (`Vec3`) and 12 edges (`connection` index pairs); `calcCentroid` averages the vertices.
  - `update` translates each vertex to the centroid, rotates it with `rotateAll` (X, then Y, then Z), translates back, then rasterises edges into the `points` buffer via `line` (parametric). `Bresline` is an unused Bresenham alternative; `rotateX/Y/Z` and `wireFrame` are unused experiments.
  - Rotation accumulates frame to frame — `corners` is mutated in place.
  - Projection is orthographic: z is simply dropped when drawing.

## SDL2 setup

- `include/SDL2` is a symlink to `/usr/local/include/SDL2` (Homebrew's `sdl2-compat` headers, via Homebrew's version-independent link), included as `#include <SDL2/SDL.h>` via `-Iinclude`.
- `lib/` is empty; `-lsdl2` resolves against Homebrew's `/usr/local/lib`. `-Llib` is there for dropping in a local library.
- Only core SDL2 is used. SDL_image / SDL_ttf / SDL_mixer are not installed; adding them needs `brew install`, headers reachable from `include/`, and extra `LINKER_FLAGS`.
- Use portable `<cmath>` calls (`std::sin`, `std::sqrt`, …), not MSVC-only `std::sinf` etc. — the project was originally a Visual Studio project.

## VS Code

`.vscode/tasks.json` defines a default `build` task that runs `make`; `.vscode/launch.json` debugs `build/debug/play` with lldb (runs the build task first).
