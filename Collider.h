#ifndef COLLIDER_H
#define COLLIDER_H

#include <vector>

#include "Vector3.h"
#include "Component.h"

class SphereCollider;
class BoxCollider;
class FrustrumCollider;
class Bounds;

class Collider : public Component
{
public:
	Collider();

	bool isColliding(Collider* collider);
	bool isColliding(Bounds* bounds);

	~Collider();

protected:
	bool enabled;
	bool isTrigger;

private:
	virtual bool isCollidingWithSphere(SphereCollider* other) = 0;
	virtual bool isCollidingWithBox(BoxCollider* other) = 0;
	virtual bool isCollidingWithFrustrum(FrustrumCollider* other) = 0;
	virtual bool isCollidingWithBounds(Bounds* other) = 0;
};

#endif