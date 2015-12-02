#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/Game.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Core;
using namespace Math;
using namespace Utility;

//Window is static so that it can be easily accessed by static friends like Input and Screen
#ifdef GL_SUPPORT
GLFWwindow* Game::glWindow;
#endif

#ifdef D3D_SUPPORT
//Statics
ID3D11Device* Game::device;
ID3D11DeviceContext* Game::deviceContext;
HINSTANCE Game::hAppInst;
HWND Game::hMainWind;

//Rip the HINSTANCE of the exe out of the MS linker
//Super hack!
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST ((HINSTANCE)&__ImageBase);

//Setup a Global Window Callback for the windows api
namespace { Game* game = 0; }

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->MsgProc(hwnd, msg, wParam, lParam);
}

#endif

std::vector<GameObject*> Game::gameObjects;

Game::Game()
{
	//Seed randoms for later
	srand((unsigned int)time(0));

#ifdef D3D_SUPPORT
	hAppInst = HINST;
	driverType = D3D_DRIVER_TYPE_HARDWARE;
	enable4xMsaa = false;
	hMainWind = 0;
	msaa4xQuality = 0;

	device = 0;
	deviceContext = 0;
	swapChain = 0;
	depthStencilBuffer = 0;
	renderTargetView = 0;
	depthStencilView = 0;

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	game = this;
#endif

}

int Game::run()
{
#ifdef GL_SUPPORT
	glWindow = glfwGetCurrentContext();
#endif

#ifdef D3D_SUPPORT
	MSG msg = { 0 };
#endif

	const int ticksPerSecond = 60;
	const int skipTicks = (int)(1000.0f / ticksPerSecond);
	const int maxFrameskip = 5;

	long long nextGameTick = GameTime::GetMillisSinceStart();

	running = true;
	int loops;

	long long frames = 0;
	long long lastTime = 0;
	long long timeInterval = 0;

	while (running)
	{
		GameTime::frameStart();

		//Handle windows messages
#ifdef D3D_SUPPORT
		if (msg.message == WM_QUIT)
		{
			running = false;
			break;
		}

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
#endif

		handleInput();

		//Calculate FPS
		timeInterval = GameTime::GetMillisSinceStart() - lastTime;

		if (timeInterval > 1000)
		{
			float fps = frames / (float)(timeInterval / 1000);
			//std::cout << "FPS: " << fps << std::endl;

			lastTime = GameTime::GetMillisSinceStart();
			frames = 0;
		}

		update();
		
		//Update physics "ticksPerSecond" times per second
		loops = 0;
		while (GameTime::GetMillisSinceStart() > nextGameTick && loops < maxFrameskip)
		{
			PhysicsManager::Update();

			nextGameTick += skipTicks;
			loops++;
		}		

		frames++;

		render();

		GameInputManager::Update();

		//See which Game Objects need deletion
		for (unsigned int i = 0; i < GameObject::getGameObjects().size(); i++)
		{
			GameObject* gameObject = GameObject::getGameObjects().at(i);
			if (gameObject->toDestroy)
				delete gameObject;
		}

		GameTime::frameEnd();
	}

#ifdef GL_SUPPORT
	endGL();
#endif

#ifdef D3D_SUPPORT
	endD3D();
#endif

	return 0;
}

bool Game::init()
{
	if (!Logger::Init())
	{
		std::cout << "Failure initializing the logger!" << std::endl;
		return false;
	}

	if(!Screen::Init())
	{
		std::cout << "Failure initializing the Screen!" << std::endl;
		return false;
	}

#ifdef D3D_SUPPORT
	if (!initD3D())
	{
		std::cout << "Failure initializing DirectX!" << std::endl;
		return false;
	}
#else
	if (!initGL())
	{
		std::cout << "Failure initializing OpenGL!" << std::endl;
		return false;
	}
#endif

	//Init managers
	Graphics::PrimitiveManager::Initialize();
#ifdef GL_SUPPORT
	Graphics::RenderingManager::Initialize(nullptr, nullptr);
#endif
#ifdef D3D_SUPPORT
	Graphics::RenderingManager::Initialize(device, deviceContext);
#endif
	PhysicsManager::Initialize();
	GameInputManager::Initialize();

	return true;
}

//Private methods

void Game::update()
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->Update();
}

//This render will be called by the game loop and wraps the super class's
//Render in the proper settings for the given rendering API
void Game::render()
{
#ifdef D3D_SUPPORT
	startRenderD3D();
#endif

	//Render each game object
	for (unsigned int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->OnRender();

	Graphics::RenderingManager::Render();

#ifdef D3D_SUPPORT
	swapBuffersD3D();
#else
	Camera* camera = Camera::GetActiveCamera();
	Graphics::PrimitiveManager::DrawPrimitives(camera->GetViewMatrix(), camera->GetProjectionMatrix());
	swapBuffersGL();
	Graphics::PrimitiveManager::ClearPrimitives();
#endif
}

void Game::handleInput()
{
#ifdef D3D_SUPPORT
	handleInputWindows();
#else
	handleInputGLFW();
#endif
}

#ifdef GL_SUPPORT
bool Game::initGL()
{
	glWindow = glfwGetCurrentContext();

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		return false;

	//Set Vsync
	if (Graphics::GraphicsSettings::VSync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//OpenGL initialization
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

#ifdef BRICKWARE_DEBUG
	// Enable the debugging layer of OpenGL
	// GL_DEBUG_OUTPUT - Faster version but not useful for breakpoints
	// GL_DEBUG_OUTPUT_SYNCHRONUS - Callback is in sync with errors, so a breakpoint
	// can be placed on the callback in order to get a stacktrace for the GL error.
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLPrintErrorCallback, NULL);
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

	//Mouse Buttons
	if(glfwGetMouseButton(glWindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		Input::buttons[MouseButton::leftButton] = true;
	else
		Input::buttons[MouseButton::leftButton] = false;

	if(glfwGetMouseButton(glWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		Input::buttons[MouseButton::rightButton] = true;
	else
		Input::buttons[MouseButton::rightButton] = false;

	if(glfwGetMouseButton(glWindow, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
		Input::buttons[MouseButton::middleButton] = true;
	else
		Input::buttons[MouseButton::middleButton] = false;

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

	if (glfwGetKey(glWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_REPEAT)
		Input::keys[KeyCode::shift] = true;
	else
		Input::keys[KeyCode::shift] = false;

	//F Keys

	if (glfwGetKey(glWindow, GLFW_KEY_F1) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_F1) == GLFW_REPEAT)
		Input::keys[KeyCode::F1] = true;
	else
		Input::keys[KeyCode::F1] = false;
	if (glfwGetKey(glWindow, GLFW_KEY_F2) == GLFW_PRESS || glfwGetKey(glWindow, GLFW_KEY_F2) == GLFW_REPEAT)
		Input::keys[KeyCode::F2] = true;
	else
		Input::keys[KeyCode::F2] = false;
}

void Game::swapBuffersGL()
{
	glfwSwapBuffers(glWindow);
}

void Game::endGL()
{
	glfwTerminate();
}
#endif

#ifdef D3D_SUPPORT
bool Game::initD3DWindow()
{
	//Call our defined hack to rip the hInstance out of the linker
	hAppInst = HINST;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "D3DWindow";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, "RegisterClass Failed.", 0, 0);
		return false;
	}

	//Calculate some window dimensions
	RECT windowRect = { 0, 0, Screen::getWidth(), Screen::getHeight() };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);
	int width = windowRect.right - windowRect.left;
	int height = windowRect.bottom - windowRect.top;

	hMainWind = CreateWindow("D3DWindow", "DirectX Brickware-Engine Magic",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hAppInst, 0);

	if (!hMainWind)
	{
		MessageBox(0, "Failed to Create DirectX Window.", 0, 0);
		return false;
	}

	ShowWindow(hMainWind, SW_SHOW);
	ShowCursor(false);
	UpdateWindow(hMainWind);
	
	return true;
}

LRESULT Game::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_LBUTTONDOWN)
		Input::buttons[MouseButton::leftButton] = true;
	else
		Input::buttons[MouseButton::leftButton] = false;

	if (msg == WM_RBUTTONDOWN)
		Input::buttons[MouseButton::rightButton] = true;
	else
		Input::buttons[MouseButton::rightButton] = false;

	if (msg == WM_MBUTTONDOWN)
		Input::buttons[MouseButton::middleButton] = true;
	else
		Input::buttons[MouseButton::middleButton] = false;

	switch (wParam)
	{
	case 0x41:
		if (msg == WM_KEYDOWN)
			Input::keys[KeyCode::a] = true;
		else if (msg == WM_KEYUP)
			Input::keys[KeyCode::a] = false;
		return 0;
	case 0x44:
		if (msg == WM_KEYDOWN)
			Input::keys[KeyCode::d] = true;
		else if (msg == WM_KEYUP)
			Input::keys[KeyCode::d] = false;
		return 0;
	case 0x53:
		if (msg == WM_KEYDOWN)
			Input::keys[KeyCode::s] = true;
		else if (msg == WM_KEYUP)
			Input::keys[KeyCode::s] = false;
		return 0;
	case 0x57:
		if (msg == WM_KEYDOWN)
			Input::keys[KeyCode::w] = true;
		else if (msg == WM_KEYUP)
			Input::keys[KeyCode::w] = false;
		return 0;

	case VK_SPACE:
		if (msg == WM_KEYDOWN)
			Input::keys[KeyCode::space] = true;
		else if (msg == WM_KEYUP)
			Input::keys[KeyCode::space] = false;
		return 0;

	case VK_ESCAPE:
		if (msg == WM_KEYDOWN)
			Input::keys[KeyCode::escape] = true;
		else if (msg == WM_KEYUP)
			Input::keys[KeyCode::escape] = false;
		return 0;

	case VK_SHIFT:
		if (msg == WM_KEYDOWN)
			Input::keys[KeyCode::shift] = true;
		else if (msg == WM_KEYUP)
			Input::keys[KeyCode::shift] = false;
		return 0;

	case VK_F1:
		if (msg == WM_KEYDOWN)
			Input::keys[KeyCode::F1] = true;
		else if (msg == WM_KEYUP)
			Input::keys[KeyCode::F1] = false;
		return 0;

	case VK_F2:
		if (msg == WM_KEYDOWN)
			Input::keys[KeyCode::F2] = true;
		else if (msg == WM_KEYUP)
			Input::keys[KeyCode::F2] = false;
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool Game::initD3D()
{
	if (!initD3DWindow())
	{
		cerr << "Error creating D3D Window!" << endl;
		return false;
	}

	UINT createDeviceFlags = 0;

	// Do we want a debug device?
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//Setup a swap chain desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = Screen::getWidth();
	swapChainDesc.BufferDesc.Height = Screen::getHeight();
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hMainWind;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	if (enable4xMsaa)
	{
		// Set up 4x MSAA
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = msaa4xQuality - 1;
	}
	else
	{
		// No MSAA
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	//Create device and swap chain 
	featureLevel = D3D_FEATURE_LEVEL_9_1;//about to be overwritten
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		0, driverType, 0, createDeviceFlags,
		0, 0, D3D11_SDK_VERSION, 
		&swapChainDesc, &swapChain, 
		&device, &featureLevel, &deviceContext);

	if (FAILED(hr))
	{
		MessageBox(0, "D3D11CreateDevice Failed!", 0, 0);
		return false;
	}

	HR(device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa4xQuality));
	assert(msaa4xQuality > 0);

	onResize();

	return true;
}

void Game::onResize()
{
	ReleaseMacro(renderTargetView);
	ReleaseMacro(depthStencilView);
	ReleaseMacro(depthStencilBuffer);

	//Resize swap chain
	HR(swapChain->ResizeBuffers(
		1, Screen::getWidth(), Screen::getHeight(),
		DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(device->CreateRenderTargetView(backBuffer, 0, &renderTargetView));
	ReleaseMacro(backBuffer);

	//Need a description of texture for the depth stencil
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = Screen::getWidth();
	depthStencilDesc.Height = Screen::getHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	if (enable4xMsaa)
	{
		// Turn on 4x MultiSample Anti Aliasing
		// This must match swap chain MSAA values
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = msaa4xQuality - 1;
	}
	else
	{
		// No MSAA
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	//Create a raster description
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	ID3D11RasterizerState* rasterState;

	// Create the depth/stencil buffer and corresponding view
	HR(device->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer));
	HR(device->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView));
	
	//Create rasterizer state
	HR(device->CreateRasterizerState(&rasterDesc, &rasterState));

	// Bind these views to the pipeline, so rendering actually
	// uses the underlying textures
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	//Set rasterizer
	deviceContext->RSSetState(rasterState);

	// Update the viewport and set it on the device
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)Screen::getWidth();
	viewport.Height = (float)Screen::getHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	deviceContext->RSSetViewports(1, &viewport);
}

void Game::handleInputWindows()
{
	//Handle mouse
	POINT mousePos;
	Vector2 newMousePos;

	//We only want to modify the mouse input if our window has focus
	if (GetActiveWindow() == hMainWind)
	{
		if (GetCursorPos(&mousePos))
		{
			if (ScreenToClient(hMainWind, &mousePos))
				newMousePos = Vector2((float)mousePos.x, (float)mousePos.y);
		}
	}
	Input::mousePosition = newMousePos;
}

void Game::startRenderD3D()
{
	//Clear the device context and setup the depth stencil
	const float cornflowerBlue[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	deviceContext->ClearRenderTargetView(renderTargetView, cornflowerBlue);
	deviceContext->ClearDepthStencilView(depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);
}

void Game::swapBuffersD3D()
{
	HR(swapChain->Present(0, 0));
}

void Game::endD3D()
{

}
#endif


Game::~Game()
{
	Graphics::PrimitiveManager::Destroy();
	Graphics::RenderingManager::Destroy();
	PhysicsManager::Destroy();
	GameInputManager::Destroy();
	Logger::Close();
}
