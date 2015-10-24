#ifndef SCREEN_H
#define SCREEN_H

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Graphics Headers
#ifdef D3D_SUPPORT
#include <windows.h>
#include <WindowsX.h>
#include <d3d11.h>
#include <assert.h>
#include "BrickwareGraphics/dxerr.hpp"
#endif

#ifdef GL_SUPPORT
#include <GL/glew.h>

#ifdef _WIN32
#include <windows.h>
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#endif

//Project headers
#include "BrickwareGraphics/GraphicsSettings.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"
#include "BrickwareGraphics/GLDebugCallback.hpp"

namespace Brickware
{
	namespace Graphics
	{
		/* Static class describing the state of the game screen
		 * 
		 * This class is for the user to easily be able to read the width, 
		 * height, aspect ratio and other states of their game's screen.
		 */
		class BRICKWARE_GRAPHICS_API Screen
		{
		public:
			/* Init's the screen subsystems so we know how to handle the
			 * window under the hood
			 */
			static bool Init();

			/* Gets the screen's width.
			 * @return The screen's width in pixels
			 */
			static int GetWidth();

			/* Gets the screen's height.
			* @return The screen's height in pixels.
			*/
			static int GetHeight();

			/* Get's the screen's aspect ratio.
			 * @return The screen's aspect ratio as a float.
			 */
			static float GetAspectRatio();

			/* Get whether or not the screen is fullscreen
			 * @return Whether or not the screen is fullscreeen
			 */
			static bool GetFullscreen();

			/* Set the screen's resolution
			 * @width The screen's new width in pixels
			 * @height The screen's new height in pixels
			 * @fullscreen Whether or not the window will be fullscreen
			 */
			static void SetResolution(int width, int height, bool fullscreen);

		private:
			static int width;
			static int height;
			static float aspectRatio;
			static bool fullscreen;

			static void(*SetResolutionPtr)(int width, int height, bool fullscreen);

#ifdef GL_SUPPORT
			static GLFWwindow* glWindow;
			static bool InitGL();
			static void SetResolutionGL(int width, int height, bool fullscreen);
#endif

#ifdef D3D_SUPPORT
			static bool InitD3D();
			static void SetResolutionD3D(int width, int height, bool fullscreen);
#endif
		};
	}
}
#endif