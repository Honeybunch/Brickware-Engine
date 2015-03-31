#define BRICKWARE_CORE_EXPORTS

#include "Collision.h"

#include "Rigidbody.h"
#include "Collider.h"

Rigidbody* Collision::getThisRigidbody()	{ return thisRigidbody; }
Collider* Collision::getThisCollider()		{ return thisCollider; }
Rigidbody* Collision::getOtherRigidbody()	{ return otherRigidbody; }
Collider* Collision::getOtherCollider()		{ return otherCollider; }
Vector3	Collision::getMTV()	{ return MTV; }
std::vector<Vector3>	Collision::getPointsOfCollision() { return pointsOfCollision; }

void Collision::setThisRigidbody(Rigidbody* rigidbody)	{ this->thisRigidbody = rigidbody; }
void Collision::setThisCollider(Collider* collider)		{ this->thisCollider = collider; }
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

bool Collision::operator==(Collision other)
{
	//Quick collider tests
	if (other.getOtherCollider()->getGameObject() != getOtherCollider()->getGameObject())
		return false;
	if (other.getThisCollider()->getGameObject() != getThisCollider()->getGameObject())
		return false;

	//Test otherRigidbodies
	if (other.getOtherRigidbody()->getGameObject() != getOtherRigidbody()->getGameObject())
		return false;
	if (other.getThisRigidbody()->getGameObject() != getThisRigidbody()->getGameObject())
		return false;
	if (other.getOtherRigidbody()->getVelocity() != getOtherRigidbody()->getVelocity())
		return false;
	if (other.getThisRigidbody()->getVelocity() != getThisRigidbody()->getVelocity())
		return false;
	if (other.getOtherRigidbody()->getAngularVelocity() != getOtherRigidbody()->getAngularVelocity())
		return false;
	if (other.getThisRigidbody()->getAngularVelocity() != getThisRigidbody()->getAngularVelocity())
		return false;
	if (other.getOtherRigidbody()->getCenterOfMass() != getOtherRigidbody()->getCenterOfMass())
		return false;
	if (other.getThisRigidbody()->getCenterOfMass() != getThisRigidbody()->getCenterOfMass())
		return false;

	return true;
}

Collision::~Collision(){}