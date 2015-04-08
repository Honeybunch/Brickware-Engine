#ifndef SCREEN_H
#define SCREEN_H

//DLL Header
#include "BrickwareCoreDLL.h"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API Screen
		{
			friend class Game;

		public:
			static int getWidth();
			static int getHeight();

			static float getAspectRatio();

		private:
			static int width;
			static int height;
		};
	}
}
#endif