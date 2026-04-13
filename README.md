# OpenGL Rotating Cube Demo

A simple 3D rotating cube application in C using OpenGL, GLFW, and GLEW. The cube's sides change color as it rotates.

## Prerequisites

This demo requires an X11 display server for the devcontainer. Make sure your devcontainer is set up to forward X11 graphics.

For detailed X11 configuration and setup information, see: [codespaces-GUI-CE](https://github.com/raymond-chetty/codespaces-GUI-CE) It is included as a submodule in this project.

## Install Required Packages

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libglfw3-dev libglew-dev libglm-dev xorg-dev
```

## Build Instructions

```bash
cd codespaces-blank
mkdir -p build
cd build
cmake ..
make
```

## Run the Application

```bash
./bin/cube_demo
```

The window will display a rotating 3D cube with colors that change over time. Press `ESC` or close the window to exit.

## Project Structure

- `main.c` - Complete demo application with cube rendering and color animation
- `CMakeLists.txt` - Build configuration
- `README.md` - This file

## Features

- 3D rotating cube with smooth animation
- Dynamic color changes on cube faces
- Clean, readable C code
- Minimal dependencies (just GLFW, GLEW, and GLM)

## Attribution

This project was created by GitHub Copilot (powered by Claude 3.5 Haiku).
