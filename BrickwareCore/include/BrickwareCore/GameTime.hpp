#ifndef GAMETIME_H
#define GAMETIME_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//System Level Headers
#include <chrono>
#include <iostream>

namespace Brickware
{
	namespace Core
	{
		/* A Simple static class used by <Game> to record relevant times.
		 * 
		 * This class is to keep track of the Delta Time between frames 
		 * as well as the milliseconds since the program started. 
		 */
		class GameTime
		{
			friend class Game;

		public:
			/* Gets the time that the last frame took in float format.
			 * @returns The delta time in float format.
			 */
			static BRICKWARE_CORE_API float GetDeltaTime();

			/* Gets the time that the last fixed update frame took
			 * @returns the fixed delta time in float format.
			 */
			static BRICKWARE_CORE_API float GetFixedDeltaTime();

			/* Gets the age of the program in milliseconds.
			 * @returns The number of milliseconds since program start.
			 */
			static BRICKWARE_CORE_API long long GetMillisSinceStart();

		private:
			static long long startTime;
			static long long frameStartTime;
			static long long fixedFrameStartTime;

			static float deltaTime;
			static float fixedDeltaTime;

			static void frameStart();
			static void frameEnd();

			static void fixedFrameStart();
			static void fixedFrameEnd();
		};
	}
}
#endif