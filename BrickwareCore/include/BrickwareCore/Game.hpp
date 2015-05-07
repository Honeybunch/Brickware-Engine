#ifndef GAME_H
#define GAME_H

//Pre-Include Defines
#define _USE_MATH_DEFINES

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

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

//Other Brickware Project Headers
#include "BrickwareGraphics/Mesh.hpp"
#include "BrickwareGraphics/Shader.hpp"
#include "BrickwareGraphics/Light.hpp"
#include "BrickwareGraphics/Material.hpp"
#include "BrickwareGraphics/Texture.hpp"
#include "BrickwareGraphics/PrimitiveManager.hpp"
#include "BrickwareGraphics/Primitive.hpp"

#include "BrickwareMath/Vector3.hpp"

//System Level Headers
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

//Project Headers
#include "BrickwareCore/GameObject.hpp"
#include "BrickwareCore/Transform.hpp"
#include "BrickwareCore/Camera.hpp"
#include "BrickwareCore/Settings.hpp"
#include "BrickwareCore/Input.hpp"
#include "BrickwareCore/Screen.hpp"
#include "BrickwareCore/SphereCollider.hpp"
#include "BrickwareCore/BoxCollider.hpp"
#include "BrickwareCore/MeshRenderer.hpp"
#include "BrickwareCore/GameTime.hpp"
#include "BrickwareCore/PhysicsManager.hpp"
#include "BrickwareCore/Debug.hpp"
#include "BrickwareCore/PointLight.hpp"
#include "BrickwareCore/FirstPersonController.hpp"
#include "BrickwareCore/GameInputManager.hpp"

namespace Brickware
{
	namespace Core
	{
#ifdef _WIN32
		template class BRICKWARE_CORE_API std::vector < GameObject* > ;
#endif

		/* The Game class that needs to be extended to have a functional game
		 *
		 * The Game has plenty of subsystems that it handles for you such as 
		 * rendering, input, windowing, and more. 
		 */
		class BRICKWARE_CORE_API Game
		{
			//Remove more of these by having a class to store the device contexts
			friend class Input;
			friend class GameObject;

			friend class Camera;
			friend class MeshRenderer;

		public:
			/* Constructor
			 * @windowWidth The width of the window you want your game to be in.
			 * @windowHeight The height of the window you want your game to be in.
			 */
			Game(int windowWidth, int windowHeight);

			/* Call this when you want your game to start running
			 * @returns An error code to tell you why the game stopped.
			 */
			int run();

			/* Initialize all the game's subsystems
			 * @returns True if the the game initialzed everything okay,
			 *          false if the game had problems initializing.
			 */
			virtual bool init();

			// Override to write your update logic
			virtual void updateScene() = 0;
			// Override to write your render logic
			virtual void renderScene() = 0;

			//THIS IS TEMPORARY, WILL BE MOVED
			void setCursorVisible(bool visible);

#ifdef D3D_SUPPORT
			LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
			// Destructor
			virtual ~Game();

		protected:
			//GameObjects will add themselves to this vector when they're created and remove themeselves when they're deleted
			static std::vector<GameObject*> gameObjects;
			//True while the game is running
			bool running;

		private:
			float interpolation;
			int ticks;

			void render();
			void handleInput();

			//OpenGL / GLFW3 vars and methods
#ifdef GL_SUPPORT
			static GLFWwindow* glWindow;

			bool initGL();
			void handleInputGLFW();
			void startRenderGL();
			void swapBuffersGL();
			void endGL();
#endif

			//D3D vars and methods
#ifdef D3D_SUPPORT
			static ID3D11Device* device;
			static ID3D11DeviceContext* deviceContext;

			static HINSTANCE hAppInst;
			static HWND hMainWind;

			bool enable4xMsaa;
			UINT msaa4xQuality;

			IDXGISwapChain* swapChain;
			ID3D11Texture2D* depthStencilBuffer;
			ID3D11RenderTargetView* renderTargetView;
			ID3D11DepthStencilView* depthStencilView;
			D3D11_VIEWPORT viewport;
			D3D_DRIVER_TYPE driverType;
			D3D_FEATURE_LEVEL featureLevel;

			//D3D Required methods
			bool initD3DWindow();
			void onResize();

			bool initD3D();
			void handleInputWindows();
			void startRenderD3D();
			void swapBuffersD3D();
			void endD3D();
#endif
		};
	}
}
#endif
