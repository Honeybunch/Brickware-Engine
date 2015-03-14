#define BRICKWARE_CORE_EXPORTS

#include "Collision.h"

#include "Rigidbody.h"
#include "Collider.h"


Rigidbody* Collision::getRigidbody(){ return otherRigidbody; }
Collider* Collision::getCollider(){ return otherCollider; }
Vector3 Collision::getMTV(){ return MTV; }

Collision::Collision(Rigidbody* otherRigidbody, Collider* otherCollider, Vector3 MTV)
{
	this->otherRigidbody = otherRigidbody;
	this->otherCollider = otherCollider;
	this->MTV = MTV;
}

Collision::~Collision(){}