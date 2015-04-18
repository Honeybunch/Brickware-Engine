#ifndef INPUT_H
#define INPUT_H

//DLL Header
#include "BrickwareCore\BrickwareCoreDLL.hpp"

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
#endif

//Other Brickware Project Headers
#include "BrickwareMath\Vector2.hpp"

//Project Headers
#include "BrickwareCore\Settings.hpp"

namespace Brickware
{
	namespace Core
	{
		//Enum that lists all the possible Key Codes that could be recorded.
		enum BRICKWARE_CORE_API KeyCode
		{
			a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
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

		//Enum that lists all the possible Mouse Buttons that could be recorded.
		enum BRICKWARE_CORE_API MouseButton
		{
			leftButton, rightButton, middleButton
		};

		/* Static class that keeps track of user input.
		 * 
		 * This is the class that you'd ask for things like which keys are pressed down
		 * or where the mouse is on the screen.
		 */
		class BRICKWARE_CORE_API Input
		{
			friend class Game;

		public:
			/* Checks the state of the given <KeyCode>.
			 * @returns True if the key is down,
			 *          false is the key is up.
			 */
			static bool getKeyDown(KeyCode key);

			/* Checks the state of the given <KeyCode>.
			 * @returns True if the key is up,
			 *          false is the key is down.
			 */
			static bool getKeyUp(KeyCode key);
			
			/* Checks the state of the given <MouseButton>.
			 * @returns True if the button is down,
			 *          false is the button is up.
			 */
			static bool getMouseButtonDown(MouseButton button);
			
			/* Checks the state of the given <MouseButton>.
			 * @returns True if the button is up,
			 *          false is the button is down.
			 */
			static bool getMouseButtonUp(MouseButton button);

			/* Gets the position of the mouse on the screen in pixels.
			 * @returns A <Vector2> containing the mouse's position in pixels .
			 */
			static Math::Vector2 getMousePosition();

			/* Sets the position of the mouse on the screen.
			 * @pos The position on the screen in pixels that you want the mouse to be at.
			 */
			static void setMousePosition(Math::Vector2 pos);

		private:
			static bool keys[77];
			static bool buttons[3];
			static Math::Vector2 mousePosition;

#ifdef GL_SUPPORT
			static void setMousePositionGLFW(Math::Vector2 pos);
#endif
#ifdef D3D_SUPPORT
			static void setMousePositionWindows(Math::Vector2 pos);
#endif
		};
	}
}
#endif