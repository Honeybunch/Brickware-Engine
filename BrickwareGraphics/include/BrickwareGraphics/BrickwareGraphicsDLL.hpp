#ifndef BRICKWARE_GRAPHICS_DLL_H
#define BRICKWARE_GRAPHICS_DLL_H

#ifdef _WIN32

#ifdef BRICKWARE_GRAPHICS_EXPORTS
#	define BRICKWARE_GRAPHICS_API __declspec(dllexport)
#else
#	define BRICKWARE_GRAPHICS_API __declspec(dllimport)
#endif

#else
#define BRICKWARE_GRAPHICS_API
#endif

#endif