#include "SphereCollider.h"
#include "BoxCollider.h"
#include "FrustrumCollider.h"
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
	FrustrumCollider* otherFrustrumCollider = dynamic_cast<FrustrumCollider*>(other);

	if (otherSphere)
		return isCollidingWithSphere(otherSphere);
	else if (otherAxisAlignedBox)
		return isCollidingWithBox(otherAxisAlignedBox);
	else if(otherFrustrumCollider)
		return isCollidingWithFrustrum(otherFrustrumCollider);

	return false;
}

bool Collider::isColliding(Bounds* bounds)
{
	return isCollidingWithBounds(bounds);
}

Collider::~Collider()
{
}
