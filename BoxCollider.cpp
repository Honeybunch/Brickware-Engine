#include "BoxCollider.h"
#include "SphereCollider.h"
#include "FrustrumCollider.h"
#include "GameObject.h"
#include "Bounds.h"

BoxCollider::BoxCollider(float width)
{
	this->width = width;
}

void BoxCollider::Start()
{
	float halfWidth = width / 2.0f;
	Vector3* center = getGameObject()->getTransform()->getPosition();

	minBound = Vector3(center->getX() - halfWidth, center->getY() - halfWidth, center->getZ() - halfWidth);
	maxBound = Vector3(center->getX() + halfWidth, center->getY() + halfWidth, center->getZ() + halfWidth);
}

//Accessors and Mutators
Vector3 BoxCollider::getMinBound(){ return minBound; }
Vector3 BoxCollider::getMaxBound(){ return maxBound; }

float BoxCollider::getWidth(){ return width; }

void BoxCollider::setMinBound(Vector3 minBound){ this->minBound = minBound; }
void BoxCollider::setMaxBound(Vector3 maxBound){ this->maxBound = maxBound; }

//Note, does not account of the center of the bounding box possibly moving; min and max need to be relative to the center of the box
bool BoxCollider::isCollidingWithSphere(SphereCollider* other)
{
	float radiusSquared = pow(other->getRadius(), 2);
	float distanceSquared = 0;

	Vector3* sphereCenter = other->getGameObject()->getTransform()->getPosition();

	//Implementation of Jim Arvo's collision algorithm

	if (sphereCenter->getX() < minBound.getX())
		distanceSquared += pow(sphereCenter->getX() - minBound.getX(), 2);
	else if (sphereCenter->getX() > maxBound.getX())
		distanceSquared += pow(sphereCenter->getX() - maxBound.getX(), 2);

	if (sphereCenter->getY() < minBound.getY())
		distanceSquared += pow(sphereCenter->getY() - minBound.getY(), 2);
	else if (sphereCenter->getY() > maxBound.getY())
		distanceSquared += pow(sphereCenter->getY() - maxBound.getY(), 2);

	if (sphereCenter->getZ() < minBound.getZ())
		distanceSquared += pow(sphereCenter->getZ() - minBound.getZ(), 2);
	else if (sphereCenter->getZ() > maxBound.getZ())
		distanceSquared += pow(sphereCenter->getZ() - maxBound.getZ(), 2);

	if (distanceSquared <= radiusSquared)
		return true;
	else
		return false;
}

//TODO: Implement
bool BoxCollider::isCollidingWithBox(BoxCollider* other)
{
	return false;
}

//TODO: Refactor to mesh collision
bool BoxCollider::isCollidingWithFrustrum(FrustrumCollider* other)
{
	return other->isColliding(this);
}

//Don't really care yet
bool BoxCollider::isCollidingWithBounds(Bounds* other)
{
	return false;
}

BoxCollider::~BoxCollider()
{
}
