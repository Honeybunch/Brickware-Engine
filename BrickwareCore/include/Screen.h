#ifndef SCREEN_H
#define SCREEN_H

#include "BrickwareCoreDLL.h"

class Screen
{
	friend class Game;

public:
	static BRICKWARE_CORE_API int getWidth();
	static BRICKWARE_CORE_API int getHeight();

	static BRICKWARE_CORE_API float getAspectRatio();

private:
	static int width;
	static int height;
};

#endif