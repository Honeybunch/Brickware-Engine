#include "GameManager.hpp"

#include "TestGame.hpp"

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;
using namespace Utility;

GameManager::GameManager()
{
	
}

void GameManager::Update()
{
	if (Input::getKeyDown(KeyCode::escape))
		TestGame::Shutdown();

#ifdef BRICKWARE_DEBUG
	if (Input::getKeyDown(KeyCode::F1) && dKeyDown == false)
	{
		Debug::Debugging = !Debug::Debugging;
		if (GameTime::GetTimeScale() != 1.0f)
			GameTime::SetTimeScale(1.0f);
		dKeyDown = true;
	}
	else if (Input::getKeyUp(KeyCode::F1))
	{
		dKeyDown = false;
	}
#endif
}


GameManager::~GameManager()
{
}
