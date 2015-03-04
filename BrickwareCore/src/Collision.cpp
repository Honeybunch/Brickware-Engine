#define BRICKWARE_CORE_EXPORTS

#include "Collision.h"

#include "Rigidbody.h"
#include "Collider.h"
#include "Vector3.h"

Rigidbody* Collision::getRigidbody(){ return otherRigidbody; }
Collider* Collision::getCollider(){ return otherCollider; }
std::vector<Vector3> Collision::getPointsOfContact(){ return pointsOfContact; }

Collision::Collision(Rigidbody* otherRigidbody, Collider* otherCollider, std::vector<Vector3> pointsOfContact)
{
	this->otherRigidbody = otherRigidbody;
	this->otherCollider = otherCollider;
	this->pointsOfContact = pointsOfContact;
}

Collision::~Collision(){}