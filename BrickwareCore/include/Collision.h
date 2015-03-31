#ifndef COLLISION_H
#define COLLISION_H

#include "BrickwareCoreDLL.h"

#include <vector>

#include "Vector3.h"

class Rigidbody;
class Collider;
class GameObject;

template class BRICKWARE_CORE_API std::vector<Vector3>;

class BRICKWARE_CORE_API Collision 
{
	friend class PhysicsManager;

public:
	GameObject* getThisObject();
	Rigidbody* getThisRigidbody();
	Collider* getThisCollider();
	GameObject* getOtherObject();
	Rigidbody* getOtherRigidbody();
	Collider* getOtherCollider();
	Vector3 getMTV();
	std::vector<Vector3> getPointsOfCollision();

	void setThisObject(GameObject* gameObject);
	void setThisRigidbody(Rigidbody* rigidbody);
	void setThisCollider(Collider* collider);
	void setOtherObject(GameObject* gameObject);
	void setOtherRigidbody(Rigidbody* rigidbody);
	void setOtherCollider(Collider* collider);
	void setMTV(Vector3 MTV);
	void setPointsOfCollision(std::vector<Vector3> pointsOfCollision);

private:
	Collision();
	~Collision();

	GameObject* thisObject;
	Rigidbody* thisRigidbody;
	Collider* thisCollider;
	GameObject* otherObject;
	Rigidbody* otherRigidbody;
	Collider* otherCollider;
	Vector3 MTV;
	std::vector<Vector3> pointsOfCollision;
};

#endif