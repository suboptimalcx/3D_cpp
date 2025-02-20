# 3D_cpp 

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