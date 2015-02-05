#ifndef GAME_H
#define GAME_H

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "Settings.h"
#include "Input.h"
#include "Screen.h"

#ifndef USE_D3D_ONLY
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
#include "Spawner.h"

using namespace std;

class Game
{
	//Remove more of these by having a class to store the device contexts
	friend class Input;
	friend class GameObject;
	friend class Shader;
	friend class Mesh;
	friend class Material;
	
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
	static vector<GameObject*> gameObjects;
	bool running;

private:
	float interpolation;
	int ticks;

	void render();
	void handleInput();

//OpenGL / GLFW3 vars and methods
#ifndef USE_D3D_ONLY
	static GLFWwindow* glWindow;

	bool initGL();
	void handleInputGLFW();
	void startRenderGL();
	void swapBuffersGL();
	void endGL();
#endif

//D3D vars and methods
#ifdef _WIN32
	HINSTANCE hAppInst;
	HWND hMainWind;
#endif

#ifdef D3D_SUPPORT
	static ID3D11Device* device;
	static ID3D11DeviceContext* deviceContext;

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

#endif