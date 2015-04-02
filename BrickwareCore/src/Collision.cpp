#define BRICKWARE_CORE_EXPORTS

#include "Collision.h"

#include "Rigidbody.h"
#include "Collider.h"
#include "GameObject.h"

using namespace Brickware;
using namespace Core;
using namespace Math;

GameObject* Collision::getThisObject()		{ return thisObject; }
Rigidbody* Collision::getThisRigidbody()	{ return thisRigidbody; }
Collider* Collision::getThisCollider()		{ return thisCollider; }
GameObject* Collision::getOtherObject()		{ return otherObject; }
Rigidbody* Collision::getOtherRigidbody()	{ return otherRigidbody; }
Collider* Collision::getOtherCollider()		{ return otherCollider; }
Vector3	Collision::getMTV()	{ return MTV; }
std::vector<Vector3>	Collision::getPointsOfCollision() { return pointsOfCollision; }

void Collision::setThisObject(GameObject* gameObject)	{ this->thisObject = gameObject; }
void Collision::setThisRigidbody(Rigidbody* rigidbody)	{ this->thisRigidbody = rigidbody; }
void Collision::setThisCollider(Collider* collider)		{ this->thisCollider = collider; }
void Collision::setOtherObject(GameObject* gameObject)	{ this->otherObject = gameObject; }
void Collision::setOtherRigidbody(Rigidbody* rigidbody) { this->otherRigidbody = rigidbody; }
void Collision::setOtherCollider(Collider* collider)	{ this->otherCollider = collider; }

void Collision::setMTV(Vector3 MTV) { this->MTV = MTV; }
void Collision::setPointsOfCollision(std::vector<Vector3> pointsOfCollision) { this->pointsOfCollision = pointsOfCollision; }

Collision::Collision()
{
	this->thisRigidbody = NULL;
	this->thisCollider = NULL;
	this->otherRigidbody = NULL;
	this->otherCollider = NULL;
}

Collision::~Collision(){}