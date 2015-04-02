#ifdef _DEBUG

#ifndef DEBUG_H
#define DEBUG_H

//DLL Header
#include "BrickwareCoreDLL.h"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API Debug
		{
		public:
			static bool Debugging;
		};
	}
}
#endif

#endif