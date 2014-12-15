#ifndef FRUSTRUMCOLLIDER_H
#define FRUSTRUMCOLLIDER_H

#include <math.h>

#include "Collider.h"

class FrustrumCollider : public Collider
{
public:
	FrustrumCollider(float zNear, float zFar, float FoV, float aspectRatio);

	virtual void Update();
	virtual void Render();

	~FrustrumCollider();

private:
	float zNear;
	float zFar;
	
	float frustrumNearWidth;
	float frustrumNearHeight;

	float frustrumFarWidth;
	float frustrumFarHeight;

	Vector3 nearBottomLeft;
	Vector3 nearBottomRight;
	Vector3 nearTopLeft;
	Vector3 nearTopRight;

	Vector3 farBottomLeft;
	Vector3 farBottomRight;
	Vector3 farTopLeft;
	Vector3 farTopRight;

	bool isCollidingWithSphere(SphereCollider* other);
	bool isCollidingWithBox(BoxCollider* other);
	bool isCollidingWithFrustrum(FrustrumCollider* other);
	bool isCollidingWithBounds(Bounds* other);
};

#endif