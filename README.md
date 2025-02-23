# 3D_cpp 
Controls are extremely basic, w/s -> move forward/backward, a/d -> look left/right, q -> quit the game.
Early doom-esque 3d ''''engine'''' made using ASCII and the terminal
<<<<<<< HEAD

Based on the Javidx9 code-it-yourself
=======
>>>>>>> c3d6c7796ca0786e322edeec72449416b77c7edf
## Project Structure

- `3D_cpp.cpp`: Source code.
- `3D_cpp.h`: Header file for standard system include files or project-specific include files.
- `CMakeLists.txt`: CMake project file to build the project.
- `CMakePresets.json`: CMake presets for different build configurations. (ninja in this case)

## Build Instructions

1. Ensure you have CMake and a C++ compiler installed.
2. Clone the repository.
3. Navigate to the project directory.
4. Run the following commands:

```sh
cmake -S . -B build
cmake --build build
```

5. when running the project make sure to set your console window to 120 width 40 height
