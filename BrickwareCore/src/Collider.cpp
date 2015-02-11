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

bool Collider::isColliding(Bounds* bounds)
{
	return isCollidingWithBounds(bounds);
}

Collider::~Collider()
{
}
