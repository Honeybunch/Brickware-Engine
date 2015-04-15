#ifndef SCREEN_H
#define SCREEN_H

//DLL Header
#include "BrickwareCore\BrickwareCoreDLL.hpp"

namespace Brickware
{
	namespace Core
	{
		/* Static class describing the state of the game screen
		 * 
		 * This class is for the user to easily be able to read the width, 
		 * height, aspect ratio and other states of their game's screen.
		 */
		class BRICKWARE_CORE_API Screen
		{
			friend class Game;

		public:
			/* Gets the screen's width.
			 * @return The screen's width in pixels
			 */
			static int getWidth();

			/* Gets the screen's height.
			* @return The screen's height in pixels.
			*/
			static int getHeight();

			/* Get's the screen's aspect ratio.
			 * @return The screen's aspect ratio as a float.
			 */
			static float getAspectRatio();

		private:
			static int width;
			static int height;
		};
	}
}
#endif