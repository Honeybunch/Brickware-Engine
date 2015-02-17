#define BRICKWARE_CORE_EXPORTS

#include "SphereCollider.h"
#include "BoxCollider.h"
#include "FrustumCollider.h"
#include "Bounds.h"

Collider::Collider()
{
	enabled = true;
	isTrigger = false;
}

bool Collider::isColliding(Collider* other)
{
	SphereCollider* otherSphere = dynamic_cast<SphereCollider*>(other);
	BoxCollider* otherAxisAlignedBox = dynamic_cast<BoxCollider*>(other);
	FrustumCollider* otherFrustumCollider = dynamic_cast<FrustumCollider*>(other);

	if (otherSphere)
		return isCollidingWithSphere(otherSphere);
	else if (otherAxisAlignedBox)
		return isCollidingWithBox(otherAxisAlignedBox);
	else if(otherFrustumCollider)
		return isCollidingWithFrustum(otherFrustumCollider);

	return false;
}

bool Collider::isColliding(Bounds bounds)
{
	return isCollidingWithBounds(bounds);
}

void Collider::calculateWorldData()
{
	//We want to take the point and normal data of this collider and 
	//Translate it into world space so that we can do real collisions on it
}

void Collider::separatingAxisTest(Vector3 axis, std::vector<Vector3> pointSet, float& min, float& max)
{
	//Return in min and max the bounds that this set of point data represents
	//When projected along the given axis
}

Collider::~Collider()
{
}
