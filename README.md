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

### Building

#### Windows

 * Install CMake GUI
 * Point to GLEW and GLFW directories
 * Point Build directory to the included Build directory (but any dir should work)
 * Configure for your platform (VS2013 tested only)
 * Generate build system
 * Build 
 
 NOTE: For VS you will need to set BrickwareTest as your startup project and set your Working Directory to "$(SolutionDir)TestBuild"

#### Linux

Linux has been tested but is still kind of shaky due to the infancy of the CMake Project. There are a few extra steps compared to Windows. This has been Tested on Debian Testing with XFCE 4, LightDM, XOrg 1.15 and AMD proprietary drivers on an AMD 7970M. This is a very unusual setup due to the hybrid graphics but hopefully your system will be less complicated and work with less hiccups. 

Anyway here are the steps

 * Clone down the repo somewhere
 * Make sure you've installed GLEW, GLFW3, CMake and Mesa.
 * In the repo create a Build directory
 * cd into Build and run cmake : `cmake ../ -DBUILD_TEST=true -DGL_SUPPORT=true -G "Unix Makefiles"`
 * CMake should generate the project just fine
 * Run `make` (and cross fingers a bit)
 * cd into TestBuild
 * For some reason CMake won't copy assets for us so we need to do it by hand with: `cp -r ..\..\Assets\* .`
 * All shaders, models, etc for the test should now be built.
 * In TestBuild you should be able to execute the test with `.\BrickwareTest`
 * The shared libraries should also be in TestBuild

#### Mac OSX

I do not have a good machine to test any Mac OSX port on. If you have the XCode command line tools installed (g++) you should be able to get away with installing GLEW and GLFW3 with Macports or Brew and follow the steps for Linux. If not then I may need to write some Objective-C shim. 

#### Logos
Artwork on this README are courtesy of [Kyle Labriola](http://www.kylelabriola.com/)
