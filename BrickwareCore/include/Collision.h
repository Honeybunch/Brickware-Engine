#ifndef COLLISION_H
#define COLLISION_H

#include "BrickwareCoreDLL.h"

#include <vector>

class Rigidbody;
class Collider;
class Vector3;

class BRICKWARE_CORE_API Collision 
{
	friend class PhysicsManager;

public:
	Rigidbody* getRigidbody();
	Collider* getCollider();
	std::vector<Vector3> getPointsOfContact();

private:
	Collision(Rigidbody* otherRigidbody, Collider* otherCollider, std::vector<Vector3> pointsOfContact);
	~Collision();

	Rigidbody* otherRigidbody;
	Collider* otherCollider;
	std::vector<Vector3> pointsOfContact;
};

#endif