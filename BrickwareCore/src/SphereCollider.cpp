#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore\SphereCollider.hpp"
#include "BrickwareCore\BoxCollider.hpp"
#include "BrickwareCore\FrustumCollider.hpp"
#include "BrickwareCore\GameObject.hpp"

#include "BrickwareMath\Bounds.hpp"

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


bool SphereCollider::isCollidingWithBox(BoxCollider* other, Collision* collision)
{
	return other->isColliding(this, NULL);
}

//TODO: refactor to mesh collision
bool SphereCollider::isCollidingWithFrustum(FrustumCollider* other)
{
	return other->isColliding(this, NULL);
}

bool SphereCollider::isCollidingWithBounds(Bounds other)
{
	float radiusSquared = pow(radius, 2);
	float distanceSquared = 0;

	Vector3 sphereCenter = getGameObject()->getTransform()->getPosition();

	//Implementation of Jim Arvo's collision algorithm
	Vector3 minBound = other.getMinBound();
	Vector3 maxBound = other.getMaxBound();

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

bool SphereCollider::isCollidingWithRay(Ray other)
{
	Vector3 localizedCenter = center - other.getOrigin();
	float distanceToCenter = localizedCenter.getMagnitude();

	Vector3 rayDirection = other.getDirection();

	//If the sphere center is behind the origin we can't just handle the projection as is
	if (Vector3::Dot(localizedCenter, rayDirection) < 0)
	{
		if (distanceToCenter > radius)
			return false;
		else if (distanceToCenter == radius) //We hit the center and could return that as the collision point
			return true;
		else //We would have to calculate the collision point
			return true;
	}
	//In this case the center of the sphere projects in front of the ray
	else
	{
		//Project center onto ray
		float projectionMag = Vector3::Dot(rayDirection, center);
		Vector3 projectedCenter = rayDirection * projectionMag;

		if ((center - projectedCenter).getMagnitude() > radius)
			return false;
		else //Could calculate point of intersection here
			return true;
	}

	return false;
}

SphereCollider::~SphereCollider()
{
}
