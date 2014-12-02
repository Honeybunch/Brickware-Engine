#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include "Collider.h"
#include "Shape.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(Vector3* center = new Vector3(0, 0, 0), float radius = 1.0f);

	bool isColliding(SphereCollider* otherCollider);

	Vector3* getCenter();
	float getRadius();

	~SphereCollider();

private:
	Vector3* center; //Measured in local space
	float radius;
};

#endif