#ifndef BRICKWARE_MATH_DLL_H
#define BRICKWARE_MATH_DLL_H

#ifdef _WIN32

#ifdef BRICKWARE_MATH_EXPORTS
#define BRICKWARE_MATH_API __declspec(dllexport)
#else
#define BRICKWARE_MATH_API __declspec(dllimport)
#endif

#else
#define BRICKWARE_MATH_API
#endif

#endif