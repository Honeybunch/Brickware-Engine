#include "SphereCollider.h"
#include "GameObject.h"

SphereCollider::SphereCollider(Vector3* center, float radius)
	: Collider()
{
	this->center = center;
	this->radius = radius;
}

bool SphereCollider::isColliding(SphereCollider* otherCollider)
{
	Transform* transform = getGameObject()->getTransform();
	Transform* otherTransform = otherCollider->getGameObject()->getTransform();

	Vector3* position = transform->getPosition();
	Vector3* otherPosition = otherTransform->getPosition();

	float distXSquared = pow(position->getX() - otherPosition->getX(), 2);
	float distYSquared = pow(position->getY() - otherPosition->getY(), 2);
	float distZSquared = pow(position->getZ() - otherPosition->getZ(), 2);

	float radiusSumSquared = pow(getRadius() + otherCollider->getRadius(), 2);

	float distanceSquared = distXSquared + distYSquared + distZSquared;

	if (radiusSumSquared > distanceSquared)
		return true;
	else
		return false;
}

Vector3* SphereCollider::getCenter(){ return center;  }
float SphereCollider::getRadius(){ return radius; }

SphereCollider::~SphereCollider()
{
}
