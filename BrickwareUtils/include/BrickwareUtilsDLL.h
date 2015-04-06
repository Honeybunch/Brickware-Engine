#ifndef BRICKWARE_UTILITY_DLL_H
#define BRICKWARE_UTILITY_DLL_H

#ifdef BRICKWARE_UTILITY_EXPORTS
#define BRICKWARE_UTILITY_API __declspec(dllexport)
#else
#define BRICKWARE_UTILITY_API __declspec(dllimport)
#endif

#endif