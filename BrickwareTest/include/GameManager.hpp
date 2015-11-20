#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "BrickwareCore/Debug.hpp"
#include "BrickwareCore/Component.hpp"

class GameManager : public Brickware::Core::Component
{
public:
	GameManager();

	virtual void Update();

	~GameManager();

private:
#ifdef BRICKWARE_DEBUG
	bool dKeyDown = false;
#endif
};

#endif