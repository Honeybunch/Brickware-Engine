#ifndef GAME_H
#define GAME_H

#include "BrickwareCoreDLL.h"

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "Settings.h"
#include "Input.h"
#include "Screen.h"

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

#include "Mesh.h"
#include "Shader.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Vector3.h"
#include "Light.h"
#include "Material.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MeshRenderer.h"
#include "Texture.h"
#include "GameTime.h"
#include "PrimitiveManager.h"
#include "Primitive.h"

using namespace std;

class Game
{
	//Remove more of these by having a class to store the device contexts
	friend class Input;
	friend class GameObject;
	friend class Shader;
	friend class Mesh;
	friend class Material; 
	friend class Texture;
	
	friend class Camera;
	friend class MeshRenderer;

public:
	BRICKWARE_CORE_API Game(int windowWidth, int windowHeight);
	
	BRICKWARE_CORE_API int run();

	virtual BRICKWARE_CORE_API bool init();

	virtual BRICKWARE_CORE_API void updateScene() = 0;
	virtual BRICKWARE_CORE_API void renderScene() = 0;

#ifdef D3D_SUPPORT
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

	virtual BRICKWARE_CORE_API ~Game();

protected:
	static BRICKWARE_CORE_API vector<GameObject*> gameObjects;
	bool running;

private:
	float interpolation;
	int ticks;

	void BRICKWARE_CORE_API render();
	void BRICKWARE_CORE_API handleInput();

//OpenGL / GLFW3 vars and methods
#ifdef GL_SUPPORT
	static BRICKWARE_CORE_API GLFWwindow* glWindow;

	bool BRICKWARE_CORE_API initGL();
	void BRICKWARE_CORE_API handleInputGLFW();
	void BRICKWARE_CORE_API startRenderGL();
	void BRICKWARE_CORE_API swapBuffersGL();
	void BRICKWARE_CORE_API endGL();
#endif

//D3D vars and methods
#ifdef D3D_SUPPORT
	static BRICKWARE_CORE_API ID3D11Device* device;
	static BRICKWARE_CORE_API ID3D11DeviceContext* deviceContext;

	static BRICKWARE_CORE_API HINSTANCE hAppInst;
	static BRICKWARE_CORE_API HWND hMainWind;

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
	bool BRICKWARE_CORE_API initD3DWindow();
	void BRICKWARE_CORE_API onResize();

	bool BRICKWARE_CORE_API initD3D();
	void BRICKWARE_CORE_API handleInputWindows();
	void BRICKWARE_CORE_API startRenderD3D();
	void BRICKWARE_CORE_API swapBuffersD3D();
	void BRICKWARE_CORE_API endD3D();
#endif

};

#endif