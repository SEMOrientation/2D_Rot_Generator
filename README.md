# 2D_Rot_Generator
A simple OpenGL renderer for generating images of a triangle rotated in 2D.

The triangle rendered is a simple scalene triangle. Images are outputted to `train` and `test` directories created in the current working directory, and are named with the angle of rotation.

Depends on GLFW -- one can use either the shared library, such as the one installed by `apt install libglfw3-dev` on Ubuntu, or the static library can be compiled and added to the `lib` directory in the source prior to compiling.

## Compiling

```
mkdir build && cd build
cmake ..
make
```

The program can then be executed as `./generator`

## Usage

Upon running the program, you will see an empty, black window. You should use this opportunity to ensure the window is using the size defined in the source. It must be a square window and should not be maximised or otherwise resized, else the images will be stretched and the triangle will be warped by the rotations.

Once the window appears correctly, press SPACE to render all rotations. The triangle will appear in the window and will rotate, with an image being saved for each rotation. When all rotations have been saved, the program will exit automatically. Alternatively, ESC can be pressed to exit without starting the render.

At any point, the window can be closed (e.g. by pressing ALT+F4) and the program will stop rendering and exit gracefully.
