#ifndef INPUT_H
#define INPUT_H

#include "Settings.h"

#ifdef D3D_SUPPORT
	#include <windows.h>
#endif

#ifdef GL_SUPPORT
#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>
#endif

#include "Vector2.h"

//Only use this line to make sure VS creates an import lib
#ifdef BRICKWARE_CORE_EXPORTS
#define BRICKWARE_CORE_API __declspec(dllexport)
#else
#define BRICKWARE_CORE_API __declspec(dllimport)
#endif

enum BRICKWARE_CORE_API KeyCode
{
	a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,
	equals, minus,
	up, down, left, right, 
	shift, enter, space, control, alt, tab, capsLock,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	del, insert, end, home, pageUp, pageDown,
	comma, period, backSlash, forwardSlash, colon, 
	doubleQuote, openingBrace, closingBrace, 
	super, escape,
	one, two, three, four, five, six, seven, eight, nine, zero
};

enum BRICKWARE_CORE_API MouseButton
{
	leftButton, rightButton, middleButton
};

class BRICKWARE_CORE_API Input
{
	friend class Game;

public:
	static bool getKeyDown(KeyCode key);
	static bool getKeyUp(KeyCode key);

	static bool getMouseButtonDown(MouseButton button);
	static bool getMouseButtonUp(MouseButton button);

	static Vector2 getMousePosition();
	static void setMousePosition(Vector2 pos);

private:
	static bool keys[77];
	static bool buttons[3];
	static Vector2 mousePosition;

#ifdef GL_SUPPORT
	static void setMousePositionGLFW(Vector2 pos);
#endif
#ifdef D3D_SUPPORT
	static void setMousePositionWindows(Vector2 pos);
#endif
};

#endif