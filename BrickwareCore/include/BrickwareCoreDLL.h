#ifndef BRICKWARE_CORE_DLL_H
#define BRICKWARE_CORE_DLL_H

#ifdef BRICKWARE_CORE_EXPORTS
#define BRICKWARE_CORE_API __declspec(dllexport)
#else
#define BRICKWARE_CORE_API __declspec(dllimport)
#endif

#endif