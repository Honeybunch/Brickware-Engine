#ifndef BRICKWARE_UTILITY_DLL_H
#define BRICKWARE_UTILITY_DLL_H

#ifdef _WIN32

#ifdef BRICKWARE_UTILITY_EXPORTS
#define BRICKWARE_UTILITY_API __declspec(dllexport)
#else
#define BRICKWARE_UTILITY_API __declspec(dllimport)
#endif

#else
#define BRICKWARE_UTILITY_API
#endif

#endif
