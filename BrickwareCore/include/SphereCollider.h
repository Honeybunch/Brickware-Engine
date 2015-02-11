#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include "Collider.h"

//Only use this line to make sure VS creates an import lib
#ifdef BRICKWARE_CORE_EXPORTS
#define BRICKWARE_CORE_API __declspec(dllexport)
#else
#define BRICKWARE_CORE_API __declspec(dllimport)
#endif

class BRICKWARE_CORE_API SphereCollider : public Collider
{
public:
	SphereCollider(Vector3* center = new Vector3(0, 0, 0), float radius = 1.0f);

	Vector3* getCenter();
	float getRadius();

	~SphereCollider();

private:
	Vector3* center; //Measured in local space
	float radius;

	bool isCollidingWithSphere(SphereCollider* other);
	bool isCollidingWithBox(BoxCollider* other);
	bool isCollidingWithFrustum(FrustumCollider* other);
	bool isCollidingWithBounds(Bounds* other);
};

#endif