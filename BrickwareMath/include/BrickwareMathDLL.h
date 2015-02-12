#ifndef BRICKWARE_MATH_DLL_H
#define BRICKWARE_MATH_DLL_H

#ifdef BRICKWARE_MATH_EXPORTS
#define BRICKWARE_MATH_API __declspec(dllexport)
#else
#define BRICKWARE_MATH_API __declspec(dllimport)
#endif

#endif