#ifndef GAME_H
#define GAME_H

//Pre-Include Defines
#define _USE_MATH_DEFINES

//DLL Header
#include "BrickwareCoreDLL.h"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>
#endif

#ifdef D3D_SUPPORT
#include <windows.h>
#include <WindowsX.h>
#include <d3d11.h>
#include <assert.h>
#include "dxerr.h"
#endif

//Other Brickware Project Headers
#include "Mesh.h"
#include "Shader.h"
#include "Light.h"
#include "Material.h"
#include "Texture.h"
#include "PrimitiveManager.h"
#include "Primitive.h"

#include "Vector3.h"

//System Level Headers
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

//Project Headers
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Settings.h"
#include "Input.h"
#include "Screen.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MeshRenderer.h"
#include "GameTime.h"
#include "PhysicsManager.h"
#include "Debug.h"
#include "PointLight.h"

namespace Brickware
{
	namespace Core
	{
		template class BRICKWARE_CORE_API std::vector < GameObject* > ;
		class BRICKWARE_CORE_API Game
		{
			//Remove more of these by having a class to store the device contexts
			friend class Input;
			friend class GameObject;

			friend class Camera;
			friend class MeshRenderer;

		public:
			Game(int windowWidth, int windowHeight);

			int run();

			virtual bool init();

			virtual void updateScene() = 0;
			virtual void renderScene() = 0;

#ifdef D3D_SUPPORT
			LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

			virtual ~Game();

		protected:
			static std::vector<GameObject*> gameObjects;
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