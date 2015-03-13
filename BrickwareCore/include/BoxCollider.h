#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "BrickwareCoreDLL.h"

#include "Collider.h"

#include <iostream>

class BRICKWARE_CORE_API BoxCollider : public Collider
{
public:
	BoxCollider();
	BoxCollider(BoxCollider& other);
	~BoxCollider();

	Vector3 getCenter();
	Vector3 getSize();

	virtual Component* Clone();
	virtual void Start();

#ifdef _DEBUG
	virtual void DebugDraw() override;
#endif

private:
	Vector3 size; //Each element of this vector represents width on one axis
	Vector3 halfSize;

	bool isCollidingWithSphere(SphereCollider* other);
	bool isCollidingWithBox(BoxCollider* other, std::vector<Vector3>& pointsOfContact);
	bool isCollidingWithFrustum(FrustumCollider* other);
	bool isCollidingWithBounds(Bounds other);
};

#endif