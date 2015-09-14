#ifndef GRAPHICS_SETTINGS_H
#define GRAPHICS_SETTINGS_H

#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"

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
		enum BRICKWARE_GRAPHICS_API RenderingAPI{
			DirectX,
			OpenGL,
			OpenGLES,
			WebGL,
			Vulcan
		};

		enum BRICKWARE_GRAPHICS_API AntiAliasing
		{
			MSAAx0
			MSAAx2,
			MSAAx4,
			MSAAx8
		};

		enum BRICKWARE_GRAPHICS_API TextureFiltering
		{
			Bilinear,
			Trilinear,
			Anisotropicx2,
			Anisotropicx4,
			Anisotropicx8,
			Anisotropicx16
		};

		class BRICKWARE_GRAPHICS_API GraphicsSettings
		{
		public:
			static RenderingAPI Renderer;
			static bool VSync;
			static AntiAliasing AA;
			static TextureFiltering TexFiltering;
		};
	}
}
#endif
