#ifndef SETTINGS_H
#define SETTINGS_H

//TEST DEFINES, PLEASE SET IN COMPILER
#define D3D_SUPPORT 
#define GL_SUPPORT

#if defined(_WIN32) && defined(D3D_SUPPORT) && defined(GL_SUPPORT)
//If we are on windows and we have asked for GL and D3D support then we can define non const bools that will allow for context switching
static bool USE_DIRECTX = true;

#define CAN_SWITCH_CONTEXT
#elif _WIN32 && defined(D3D_SUPPORT)

#define USE_D3D_ONLY
#endif

#if defined(_WIN32) && defined(D3D_SUPPORT)

#include "Windows.h"
#include "dxerr.h"
#include <string>
#include <d3d11.h>
#include <assert.h>

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// Macro for popping up a text box based
// on a failed HRESULT and then quitting (only in debug builds)
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)												\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);	\
			PostQuitMessage(0);									\
		}														\
	}														
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif

#endif

#endif