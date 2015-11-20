#ifndef TESTGAME_H
#define TESTGAME_H

#include "BrickwareCore/Game.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"
#include "BrickwareUtils/JSONParser.hpp"

#include <iostream>

#include "Spin.hpp"
#include "Spawner.hpp"
#include "GameManager.hpp"

class TestGame : public Brickware::Core::Game {
public:
  TestGame();

  virtual bool init();

  // Sets running to false and closes game
  static void Shutdown();

  ~TestGame();

private:
	static TestGame* currentGame;
};

#endif
