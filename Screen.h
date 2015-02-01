#ifndef SCREEN_H
#define SCREEN_H

class Screen
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