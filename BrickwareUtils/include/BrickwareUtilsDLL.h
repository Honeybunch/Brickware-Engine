#ifndef BRICKWARE_UTILS_DLL_H
#define BRICKWARE_UTILS_DLL_H

#ifdef BRICKWARE_UTILS_EXPORTS
#define BRICKWARE_UTILS_API __declspec(dllexport)
#else
#define BRICKWARE_UTILS_API __declspec(dllimport)
#endif

#endif