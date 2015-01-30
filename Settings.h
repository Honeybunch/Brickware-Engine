#ifndef SETTINGS_H
#define SETTINGS_H

//TEST DEFINES, PLEASE SET IN COMPILER
#define D3D_SUPPORT 
#define GL_SUPPORT

#if defined(_WIN32) && defined(D3D_SUPPORT) && defined(GL_SUPPORT)
//If we are on windows and we have asked for GL and D3D support then we can define non const bools that will allow for context switching
static bool USE_DIRECTX = false;

#define CAN_SWITCH_CONTEXT
#elif _WIN32 && defined(D3D_SUPPORT)

#define USE_D3D_ONLY
#endif


#endif