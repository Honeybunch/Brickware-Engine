#ifndef SCREEN_H
#define SCREEN_H

//Only use this line to make sure VS creates an import lib
#ifdef BRICKWARE_CORE_EXPORTS
#define BRICKWARE_CORE_API __declspec(dllexport)
#else
#define BRICKWARE_CORE_API __declspec(dllimport)
#endif

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

#endif