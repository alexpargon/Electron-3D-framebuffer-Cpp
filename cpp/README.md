# Spinning Red Cube Framebuffer

This C++ application renders a spinning red cube using OpenGL and saves the framebuffer to `/tmp/cube_framebuffer.bin` for sharing with Electron.

## Build Instructions

1. Install dependencies: `GLFW`, `GLEW`, `OpenGL`, and `CMake`.
2. Build:
   ```bash
   cd cpp
   cmake .
   make
   ```
3. Run:
   ```bash
   ./spinning_cube
   ```

## Framebuffer Sharing

The framebuffer is saved as a raw RGBA file. Electron can read `/tmp/cube_framebuffer.bin` and display the image.
