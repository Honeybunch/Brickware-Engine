#ifndef DESPAWN_H
#define DESPAWN_H

#include "BrickwareCore\Component.hpp"
#include "BrickwareCore\GameObject.hpp"
#include "BrickwareCore\Transform.hpp"
#include "BrickwareMath\Vector3.hpp"

class Despawn : public Brickware::Core::Component
{
public:
	Despawn();

	virtual void Update();

	~Despawn();
};

#endif