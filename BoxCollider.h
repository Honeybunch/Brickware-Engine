#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(float width);
	~BoxCollider();

	void Start();

	//Accessors and Mutators
	Vector3 getMinBound();
	Vector3 getMaxBound();

	float getWidth();

	void setMinBound(Vector3 minBound);
	void setMaxBound(Vector3 maxBound);

private:
	float width;

	Vector3 minBound;
	Vector3 maxBound;

	bool isCollidingWithSphere(SphereCollider* other);
	bool isCollidingWithBox(BoxCollider* other);
	bool isCollidingWithFrustrum(FrustrumCollider* other);
	bool isCollidingWithBounds(Bounds* other);
};

#endif