<img src="http://i.imgur.com/83X6Kfn.png" alt="brickware_name" height="100">
<img src="http://i.imgur.com/Qje9ZhI.png" alt="brickware_logo" height="100">

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

#### Assets

I have recently removed the Assets folder from the repo as it was adding quite a large chunk of crap data to the repo's history. I suggest editing the `TestGame.cpp` file in the BrickwareTest project to load whatever assets you have available. I will keep the shaders available as it would be a pain to write those all out but any models or textures you will have to bring in yourself. If you want to make contributions but don't want to deal with all that I can share my assets with you on Google Drive.

The structure for assets is as such:

TestBuild/

  BrickwareTest.exe
  
  Shaders/
  
    Shaders.glsl (or .cso if DX11)
    
  Textures/
  
    Textures.bmp (24bpp bmps only right now)
    
  Models/
  
    Models.obj (triangulated objs only)
    
  Data/
  
    Data.json

### Building

#### Windows

 * Install CMake GUI
 * Point to your GLEW and GLFW directories
 * Choose a Build directory that works for you
 * Configure for your platform (VS 2013 and 2015 tested only)
 * Generate build system
 * Build
 * Make sure all assets are copied into their proper directories

 NOTES:
 * For VS you will need to set BrickwareTest as your startup project and set your Working Directory to "$(SolutionDir)TestBuild"
 * I recommend VS 2013; 2015 will generate a bunch of warnings that don't appear in 2013
 * I have not tried a 64-bit build on Windows yet

#### Linux

Linux has been tested but is still kind of shaky due to the infancy of the CMake Project. There are a few extra steps compared to Windows.

This has been Tested on:
Debian Testing with XFCE 4, LightDM, XOrg 1.15 and AMD proprietary drivers on an AMD 7970M.
Antergos Linux with LTS kernel 3.14, Gnome Shell, GDM, XOrg 1.17 and the Mesa Radeon drivers on an AMD 7970M

Feel free to add any other setups you have that work or do not work

Anyway here are the steps to build

 * Clone down the repo somewhere
 * Make sure you've installed GLEW, GLFW3, CMake and Mesa or your proprietary driver.
 * In the repo create a Build directory
 * cd into Build and run cmake : `cmake ../ -DBUILD_TEST=true -DGL_SUPPORT=true -G "Unix Makefiles"`
 * CMake should generate the project just fine
 * Run `make` (and cross fingers a bit)
 * cd into TestBuild
 * Make sure all assets are copied into their proper directories
 * In TestBuild you should be able to execute the test with `.\BrickwareTest`
 * The shared libraries should also be in TestBuild

#### Mac OSX

I do not have a good machine to test any Mac OSX port on. If you have the XCode command line tools installed (g++) you should be able to get away with installing GLEW and GLFW3 with Macports or Brew and follow the steps for Linux. If not then I may need to write some Objective-C shim.

#### Logos
Artwork on this README is courtesy of [Kyle Labriola](http://www.kylelabriola.com/)
