#ifndef TESTGAME_H
#define TESTGAME_H

#include "Game.h"
#include "Spin.h"
#include "Spawner.h"

class TestGame : public Game
{
public:
	TestGame();

	virtual bool init();

	~TestGame();

private:
	virtual void updateScene() override;
	virtual void renderScene() override;
};

#endif