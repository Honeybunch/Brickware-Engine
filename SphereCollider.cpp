#include "SphereCollider.h"
#include "BoxCollider.h"
#include "FrustrumCollider.h"
#include "GameObject.h"

SphereCollider::SphereCollider(Vector3* center, float radius)
	: Collider()
{
	this->center = center;
	this->radius = radius;
}

Vector3* SphereCollider::getCenter(){ return center;  }
float SphereCollider::getRadius(){ return radius; }

bool SphereCollider::isCollidingWithSphere(SphereCollider* other)
{
	Transform* transform = getGameObject()->getTransform();
	Transform* otherTransform = other->getGameObject()->getTransform();

	Vector3 position = transform->getPosition();
	Vector3 otherPosition = otherTransform->getPosition();

	float distXSquared = pow(position.getX() - otherPosition.getX(), 2);
	float distYSquared = pow(position.getY() - otherPosition.getY(), 2);
	float distZSquared = pow(position.getZ() - otherPosition.getZ(), 2);

	float radiusSumSquared = pow(getRadius() + other->getRadius(), 2);

	float distanceSquared = distXSquared + distYSquared + distZSquared;

	if (radiusSumSquared > distanceSquared)
		return true;
	else
		return false;
}


bool SphereCollider::isCollidingWithBox(BoxCollider* other)
{
	return other->isColliding(this);
}

//TODO: refactor to mesh collision
bool SphereCollider::isCollidingWithFrustrum(FrustrumCollider* other)
{
	return other->isColliding(this);
}

bool SphereCollider::isCollidingWithBounds(Bounds* other)
{
	return false;
}

SphereCollider::~SphereCollider()
{
}
