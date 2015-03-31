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
	Rigidbody* getThisRigidbody();
	Collider* getThisCollider();
	Rigidbody* getOtherRigidbody();
	Collider* getOtherCollider();
	Vector3 getMTV();
	std::vector<Vector3> getPointsOfCollision();

	void setThisRigidbody(Rigidbody* rigidbody);
	void setThisCollider(Collider* collider);
	void setOtherRigidbody(Rigidbody* rigidbody);
	void setOtherCollider(Collider* collider);
	void setMTV(Vector3 MTV);
	void setPointsOfCollision(std::vector<Vector3> pointsOfCollision);

	bool operator==(Collision other);

private:
	Collision();
	~Collision();

	Rigidbody* thisRigidbody;
	Collider* thisCollider;
	Rigidbody* otherRigidbody;
	Collider* otherCollider;
	Vector3 MTV;
	std::vector<Vector3> pointsOfCollision;
};

#endif