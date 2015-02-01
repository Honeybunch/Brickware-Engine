#ifndef GAME_H
#define GAME_H

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>

#include "Settings.h"
#include "Input.h"
#include "Screen.h"

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
	friend class Input;
	friend class GameObject;

public:
	Game(int windowWidth, int windowHeight);
	
	int run();

	virtual bool init();
	//virtual void onResize();
	
	virtual void updateScene() = 0;
	virtual void renderScene() = 0;

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
#ifdef D3D_SUPPORT

	bool initD3D();
	void handleInputWindows();
	void startRenderD3D();
	void swapBuffersD3D();
	void endD3D();
#endif

};

#endif