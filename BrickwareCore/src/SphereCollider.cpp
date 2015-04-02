#define BRICKWARE_CORE_EXPORTS

#include "SphereCollider.h"
#include "BoxCollider.h"
#include "FrustumCollider.h"
#include "GameObject.h"
#include "Bounds.h"

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;

SphereCollider::SphereCollider(Vector3 center, float radius)
	: Collider()
{
	this->center = center;
	this->radius = radius;
}

Vector3 SphereCollider::getCenter(){ return center;  }
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
	return other->isColliding(this, NULL);
}

//TODO: refactor to mesh collision
bool SphereCollider::isCollidingWithFrustum(FrustumCollider* other)
{
	return other->isColliding(this, NULL);
}

bool SphereCollider::isCollidingWithBounds(Bounds* other)
{
	float radiusSquared = pow(radius, 2);
	float distanceSquared = 0;

	Vector3 sphereCenter = getGameObject()->getTransform()->getPosition();

	//Implementation of Jim Arvo's collision algorithm
	Vector3 minBound = other->getMinBound();
	Vector3 maxBound = other->getMaxBound();

	if (sphereCenter.getX() < minBound.getX())
		distanceSquared += pow(sphereCenter.getX() - minBound.getX(), 2);
	else if (sphereCenter.getX() > maxBound.getX())
		distanceSquared += pow(sphereCenter.getX() - maxBound.getX(), 2);

	if (sphereCenter.getY() < minBound.getY())
		distanceSquared += pow(sphereCenter.getY() - minBound.getY(), 2);
	else if (sphereCenter.getY() > maxBound.getY())
		distanceSquared += pow(sphereCenter.getY() - maxBound.getY(), 2);

	if (sphereCenter.getZ() < minBound.getZ())
		distanceSquared += pow(sphereCenter.getZ() - minBound.getZ(), 2);
	else if (sphereCenter.getZ() > maxBound.getZ())
		distanceSquared += pow(sphereCenter.getZ() - maxBound.getZ(), 2);

	if (distanceSquared <= radiusSquared)
		return true;
	else
		return false;
}

SphereCollider::~SphereCollider()
{
}
