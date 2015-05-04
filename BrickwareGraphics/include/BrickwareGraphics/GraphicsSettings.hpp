#ifndef GRAPHICS_SETTINGS_H
#define GRAPHICS_SETTINGS_H

		//If we're not on windows, we can't use DirectX
#ifndef _WIN32
#define GL_SUPPORT
#undef D3D_SUPPORT
#else

		//If neither is defined, we fall back to GL
#if !defined(D3D_SUPPORT) && !defined(GL_SUPPORT)
#define GL_SUPPORT
#undef D3D_SUPPORT
#endif

		//If we're on windows and asked for DirectX, lets no longer ask for GL
#if defined(D3D_SUPPORT) && defined(_WIN32)
#undef GL_SUPPORT
#endif

#endif

		//Special GL Macros
#ifdef GL_SUPPORT
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#endif

		//Special DirectX macros
#ifdef D3D_SUPPORT

		/*
		#include "Windows.h"
		#include "dxerr.h"
		#include <string>
		#include <d3d11.h>
		#include <assert.h>
		*/

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

namespace Brickware
{
	namespace Graphics
	{

	}
}
#endif