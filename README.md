# Brickware-Engine
A Simple 3D engine in OpenGL+DirectX and C++ modeled off of Unity3D

This is mostly for my own fulfillment and education but if you want to fork or dig through it feel free. 

I will probably not accept pull requests for things other than fixing comments or syntax.

------

## How to Build


###Dependencies

The goal is to have as few dependencies as possible with this project. Right now the list is as such

#### OpenGL
 * GLEW
 * GLFW3
 * GL core library
 
#### DirectX
 * Windows 8 SDK

### Building

#### Windows

 * Install CMake GUI
 * Point to GLEW and GLFW directories
 * Point Build directory to the included Build directory (but any dir should work)
 * Configure for your platform (VS2013 tested only)
 * Generate build system
 * Build 
 
 NOTE: For VS you will need to set BrickwareTest as your startup project and set your Working Directory to "$(SolutionDir)TestBuild"

#### Linux & Mac OS

Linux/Mac Builds have not currently been tested but should work if the dependencies are installed
Linux build steps will be added when the engine works
s