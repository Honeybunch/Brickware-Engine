#include "Game.h"

//Window is static so that it can be easily accessed by static friends like Input and Screen
#ifndef USE_D3D_ONLY
GLFWwindow* Game::glWindow;
#endif

vector<GameObject*> Game::gameObjects;

Game::Game(int windowWidth, int windowHeight)
{
	Screen::width = windowWidth;
	Screen::height = windowHeight;
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

		updateScene();

		//Update physics 25 times per second
		loops = 0;
		while (ticks > nextGameTick && loops < maxFrameskip)
		{
			//physics go here

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
	glWindow = glfwCreateWindow(Screen::width, Screen::height, "Brickware-Test", NULL, NULL);

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

	double mouseX, mouseY;

	//Mouse position
	glfwGetCursorPos(glWindow, &mouseX, &mouseY);
	Input::mousePosition = Vector2((float)mouseX, (float)mouseY);

	//Keyboard letters
	if (glfwGetKey(glWindow, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_A) == GLFW_REPEAT)
		Input::keys[KeyCode::a] = true;
	else
		Input::keys[KeyCode::a] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_B) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_B) == GLFW_REPEAT)
		Input::keys[KeyCode::b] = true;
	else
		Input::keys[KeyCode::b] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_C) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_C) == GLFW_REPEAT)
		Input::keys[KeyCode::c] = true;
	else
		Input::keys[KeyCode::c] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_D) == GLFW_REPEAT)
		Input::keys[KeyCode::d] = true;
	else
		Input::keys[KeyCode::d] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_E) == GLFW_REPEAT)
		Input::keys[KeyCode::e] = true;
	else
		Input::keys[KeyCode::e] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_F) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_F) == GLFW_REPEAT)
		Input::keys[KeyCode::g] = true;
	else
		Input::keys[KeyCode::g] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_H) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_H) == GLFW_REPEAT)
		Input::keys[KeyCode::h] = true;
	else
		Input::keys[KeyCode::h] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_I) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_I) == GLFW_REPEAT)
		Input::keys[KeyCode::i] = true;
	else
		Input::keys[KeyCode::i] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_J) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_J) == GLFW_REPEAT)
		Input::keys[KeyCode::j] = true;
	else
		Input::keys[KeyCode::j] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_K) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_K) == GLFW_REPEAT)
		Input::keys[KeyCode::k] = true;
	else
		Input::keys[KeyCode::k] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_L) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_L) == GLFW_REPEAT)
		Input::keys[KeyCode::l] = true;
	else
		Input::keys[KeyCode::l] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_M) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_M) == GLFW_REPEAT)
		Input::keys[KeyCode::m] = true;
	else
		Input::keys[KeyCode::m] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_N) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_N) == GLFW_REPEAT)
		Input::keys[KeyCode::n] = true;
	else
		Input::keys[KeyCode::n] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_O) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_O) == GLFW_REPEAT)
		Input::keys[KeyCode::p] = true;
	else
		Input::keys[KeyCode::p] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_Q) == GLFW_REPEAT)
		Input::keys[KeyCode::q] = true;
	else
		Input::keys[KeyCode::q] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_R) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_R) == GLFW_REPEAT)
		Input::keys[KeyCode::r] = true;
	else
		Input::keys[KeyCode::r] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_S) == GLFW_REPEAT)
		Input::keys[KeyCode::s] = true;
	else
		Input::keys[KeyCode::s] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_T) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_T) == GLFW_REPEAT)
		Input::keys[KeyCode::t] = true;
	else
		Input::keys[KeyCode::t] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_U) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_U) == GLFW_REPEAT)
		Input::keys[KeyCode::u] = true;
	else
		Input::keys[KeyCode::u] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_V) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_V) == GLFW_REPEAT)
		Input::keys[KeyCode::v] = true;
	else
		Input::keys[KeyCode::v] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_W) == GLFW_REPEAT)
		Input::keys[KeyCode::w] = true;
	else
		Input::keys[KeyCode::w] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_X) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_X) == GLFW_REPEAT)
		Input::keys[KeyCode::x] = true;
	else
		Input::keys[KeyCode::x] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_Y) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_Y) == GLFW_REPEAT)
		Input::keys[KeyCode::y] = true;
	else
		Input::keys[KeyCode::y] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_Z) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_Z) == GLFW_REPEAT)
		Input::keys[KeyCode::z] = true;
	else
		Input::keys[KeyCode::z] = false;

	//Special Keys
	if (glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_REPEAT)
		Input::keys[KeyCode::escape] = true;
	else
		Input::keys[KeyCode::escape] = false;

	if (glfwGetKey(glWindow, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_SPACE) == GLFW_REPEAT)
		Input::keys[KeyCode::space] = true;
	else
		Input::keys[KeyCode::space] = false;
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
