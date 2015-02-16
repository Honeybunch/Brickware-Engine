#ifndef FRUSTRUMCOLLIDER_H
#define FRUSTRUMCOLLIDER_H

#include "BrickwareCoreDLL.h"

#define USE_MATH_DEFINES

#include <math.h>

#include "Collider.h"

class BRICKWARE_CORE_API FrustumCollider : public Collider
{
public:
	FrustumCollider(float zNear, float zFar, float FoV, float aspectRatio);

	virtual void Update();
	virtual void Render();

	~FrustumCollider();

private:
	float zNear;
	float zFar;
	
	float frustumNearWidth;
	float frustumNearHeight;

	float frustumFarWidth;
	float frustumFarHeight;

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
	bool isCollidingWithFrustum(FrustumCollider* other);
	bool isCollidingWithBounds(Bounds other);
};

#endif