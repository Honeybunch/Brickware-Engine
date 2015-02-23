#ifndef TESTGAME_H
#define TESTGAME_H

#include "Game.h"
#include "Spin.h"
#include "Spawner.h"

#include <iostream>

class TestGame : public Game
{
public:
	TestGame();

	virtual bool init();

#ifdef _DEBUG
	bool dKeyDown = false;
#endif

	~TestGame();

private:
	virtual void updateScene() override;
	virtual void renderScene() override;
};

#endif