#ifndef TESTGAME_H
#define TESTGAME_H

#include "Game.h"

class TestGame : public Game
{
public:
	TestGame();

	virtual bool init();

	~TestGame();

private:
	Shader* shader;

	Mesh* sphereMesh;
	Mesh* cubeMesh;

	virtual void updateScene() override;
	virtual void renderScene() override;
};

#endif