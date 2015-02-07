#include "Input.h"

#include "Game.h"

bool Input::keys[77];
bool Input::buttons[3];
Vector2 Input::mousePosition;

bool Input::getKeyDown(KeyCode key)
{
	return keys[key];
}
bool Input::getKeyUp(KeyCode key)
{
	return !keys[key];
}

bool Input::getMouseButtonDown(MouseButton button)
{
	return buttons[button];
}
bool Input::getMouseButtonUp(MouseButton button)
{
	return !buttons[button];
}

Vector2 Input::getMousePosition(){ return mousePosition; }

void Input::setMousePosition(Vector2 pos)
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		setMousePositionWindows(pos);
	else
		setMousePositionGLFW(pos);
#elif defined(USE_D3D_ONLY)
	setMousePositionWindows(pos);
#else
	setMousePositionGLFW(pos);
#endif
}

void Input::setMousePositionGLFW(Vector2 pos)
{
	if (Game::glWindow)
		glfwSetCursorPos(Game::glWindow, pos.getX(), pos.getY());
}

void Input::setMousePositionWindows(Vector2 pos)
{
	if (GetActiveWindow() == Game::hMainWind)
	{
		POINT newMousePos;
		newMousePos.x = (LONG)pos.getX();
		newMousePos.y = (LONG)pos.getY();

		ClientToScreen(Game::hMainWind, &newMousePos);
		SetCursorPos(newMousePos.x, newMousePos.y);
	}
}