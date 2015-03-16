#define BRICKWARE_CORE_EXPORTS

#include "Collision.h"

#include "Rigidbody.h"
#include "Collider.h"


Rigidbody* Collision::getRigidbody() { return otherRigidbody; }
Collider* Collision::getCollider() { return otherCollider; }
Vector3	Collision::getMTV()	{ return MTV; }
std::vector<Vector3>	Collision::getPointsOfCollision() { return pointsOfCollision; }

void Collision::setRigidbody(Rigidbody* rigidbody) { this->otherRigidbody = rigidbody; }
void Collision::setCollider(Collider* collider)	{ this->otherCollider = collider; }
void Collision::setMTV(Vector3 MTV) { this->MTV = MTV; }
void Collision::setPointsOfCollision(std::vector<Vector3> pointsOfCollision) { this->pointsOfCollision = pointsOfCollision; }

Collision::Collision()
{
	this->otherRigidbody = NULL;
	this->otherCollider = NULL;
}

Collision::~Collision(){}