# 3D_cpp 
Controls are extremely basic, w/s -> move forward/backward, a/d -> look left/right, q -> quit the game.
Early doom-esque 3d ''''engine'''' made using ASCII and the terminal
## Preview

(assets/preview.gif)

Based on the Javidx9 code-it-yourself
## Project Structure

- `main.cpp`: Source code.
- `utils.h & utils.cpp`: Header file for standard system include files and project functions.
- `CMakeLists.txt`: CMake project file to build the project.

## Build Instructions

1. Ensure you have CMake and a C++ compiler installed.
2. Clone the repository.
3. Navigate to the project directory.
4. Run the following commands:

```sh
cmake -S . -B build
cmake --build build
```

5. when running the project make sure to set your console window to 120 width 40 height!!
