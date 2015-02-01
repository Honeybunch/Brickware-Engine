#include "Game.h"

Game::Game(int windowWidth, int windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}

int Game::run()
{
	float ticksPerSecond = 25.0f;
	int skipTicks = (int)(1000.0f / ticksPerSecond);
	float maxFrameskip = 5;

	int nextGameTick = 0;

	int loops = 0;

	running = true;
	ticks = 0;

	while (running)
	{
		ticks++;

		handleInput();

		//Update logic 25 times per second
		loops = 0;
		while (ticks > nextGameTick && loops < maxFrameskip)
		{
			updateScene();

			nextGameTick += skipTicks;
			loops++;
		}

		//Calculate interpolation
		interpolation = (float)(((float)ticks + skipTicks - nextGameTick) / (float)skipTicks);

		render();
	}

	return 0;
}

bool Game::init()
{
	bool initSuccess = true;

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		initSuccess = initD3D();
	else
		initSuccess = initGL();
#elif defined(USE_D3D_ONLY)
	initSuccess = initD3D();
#else
	initSuccess = initGL();
#endif

	return initSuccess;
}

//Private methods

//This render will be called by the game loop and wraps the super class's
//Render in the proper settings for the given rendering API
void Game::render()
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		startRenderD3D();
	else
		startRenderGL();
#elif defined(USE_D3D_ONLY)
	startRenderD3D();
#else
	startRenderGL();
#endif

	renderScene(); //Will be overridden 

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		swapBuffersD3D();
	else
		swapBuffersGL();
#elif defined(USE_D3D_ONLY)
	swapBuffersD3D();
#else
	swapBuffersGL();
#endif
}

void Game::handleInput()
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		handleInputWindows();
	else
		handleInputGLFW();
#elif defined(USE_D3D_ONLY)
	handleInputWindows();
#else
	handleInputGLFW();
#endif
}

#ifndef USE_D3D_ONLY
bool Game::initGL()
{
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Attempt initialization
	if (!glfwInit())
		return false;

	//Create window
	glWindow = glfwCreateWindow(windowWidth, windowHeight, "Brickware-Test", NULL, NULL);

	if (!glWindow)
	{
		glfwTerminate();
		return false;
	}

	//Make Context
	glfwMakeContextCurrent(glWindow);

	//OpenGL initialization
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

#ifdef _WIN32
	glewInit();
#endif

	return true;
}

void Game::handleInputGLFW()
{
	glfwPollEvents();
}

void Game::startRenderGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Game::swapBuffersGL()
{
	glfwSwapBuffers(glWindow);

	//TODO: populate input class
}

void Game::endGL()
{
	glfwTerminate();
}
#endif

#ifdef D3D_SUPPORT
bool Game::initD3D()
{
	return true;
}

void Game::handleInputWindows()
{
	//TODO: populate input class
}

void Game::startRenderD3D()
{

}

void Game::swapBuffersD3D()
{

}

void Game::endD3D()
{

}
#endif


Game::~Game()
{
}
