#ifndef TESTGAME_H
#define TESTGAME_H

#include "BrickwareCore/Game.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"
#include "BrickwareUtils/JSONParser.hpp"

#include <iostream>

#include "Spin.hpp"
#include "Spawner.hpp"

class TestGame : public Brickware::Core::Game
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