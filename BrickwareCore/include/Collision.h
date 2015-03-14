#ifndef COLLISION_H
#define COLLISION_H

#include "BrickwareCoreDLL.h"

#include <vector>

#include "Vector3.h"

class Rigidbody;
class Collider;

template class BRICKWARE_CORE_API std::vector<Vector3>;

class BRICKWARE_CORE_API Collision 
{
	friend class PhysicsManager;

public:
	Rigidbody* getRigidbody();
	Collider* getCollider();
	Vector3 getMTV();

private:
	Collision(Rigidbody* otherRigidbody, Collider* otherCollider, Vector3 MTV);
	~Collision();

	Rigidbody* otherRigidbody;
	Collider* otherCollider;
	Vector3 MTV;
};

#endif