# GTA Base Project

## Overview

**GTA Base** is a mod menu for GTA V.

## Project Structure

- **`common/`**: Contains code shared between all other components. This includes utilities for string formatting, concurrency controls, virtual file system management, exception logging, memory operations, and Windows API wrappers. It serves as the foundational library that abstracts away complex or repetitive logic from the main feature modules.
- **`imfont/`**: A component to help with loading fonts and merging Font Awesome icons into ImGui font atlases. It provides utilities for managing custom fonts and icons used in the UI.
- **`injector/`**: A standalone executable responsible for attaching natively to the game. It's a simple injector using VirtualAlloc and LoadLibrary.
- **`native_gen/`**: A Python script to generate the files needed to make the native invoker work.
- **`rage/`**: Internal classes from GTA V.
- **`menu/`**: The main menu component compiled into the dll that's injected into the game.

## Toolchain & Dependencies

- **Compiler**: Exclusively natively supported by the MSVC (Microsoft Visual C++) toolchain due to extensive Win32 API and memory manipulation techniques utilized. 
- **C++ Version**: Targets C++23.
- **Build System**: CMake 3.16+ coupled heavily with `get_cpm.cmake` (CMake Package Manager) for rapid, implicit vendor dependency fetching directly from GitHub rather than manual vendoring.
- **Third-Party Integrations**: Relies on components defined in `vendor/` CMake modules including, but not limited to:
  - `fmt` (string formatting)
  - `imgui` (graphical interface)
  - `polyhook_2_0` (function detouring and IAT/VMT hooking)
  - `discord-rpc` (user status integrations)
  - `spdlog` (fast, concurrent logging)

## Building the Project

### Prerequisites
- Install CMake (>= 3.16)
- Install Visual Studio 2022+ with "Desktop development with C++" workload (incorporating MSBuild, MSVC, and Windows SDK).

### Build Procedure
1. Clone the project to your local drive.
2. From the project root, configure with CMake. If using CLion, ensure the toolchain selected is MSVC.
   ```bash
   cmake -B cmake-build-release -G "Visual Studio 17 2022" -T v143
   ```
3. Build the project targets natively via CMake:
   ```bash
   cmake --build cmake-build-release --config Release
   ```
4. Find the built artifacts within your generated binary folder.
   - Run the compiled `injector.exe` to inject `menu.dll` directly into the target process.
