#ifndef DESPAWN_H
#define DESPAWN_H

#include "Component.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector3.h"

class Despawn : public Component
{
public:
	Despawn();

	virtual void Update();

	~Despawn();
};

#endif