#ifdef _DEBUG

#ifndef DEBUG_H
#define DEBUG_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

namespace Brickware
{
	namespace Core
	{
		/* Static class containing states and methods to aid in debugging
		 *
		 * Right now this just stores a bool that returns whether or not the program should
		 * be giving debug output
		 */
		class BRICKWARE_CORE_API Debug
		{
		public:
			//Global flag to determine whether the game is in a debugging mode
			static bool Debugging;
		};
	}
}
#endif

#endif