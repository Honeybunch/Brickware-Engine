#ifndef COLLIDER_H
#define COLLIDER_H

#include "BrickwareCoreDLL.h"

#include <vector>

#include "Vector3.h"
#include "Matrix3.h"

class SphereCollider;
class BoxCollider;
class FrustumCollider;
class Bounds;

#include "Component.h"

//Used to avoid warnings about exporting std::vectors
template class BRICKWARE_CORE_API std::vector < Vector3 > ;

class BRICKWARE_CORE_API Collider : public Component
{
public:
	Collider();

	bool isColliding(Collider* collider);
	bool isColliding(Bounds bounds);

	~Collider();

protected:
	bool enabled;
	bool isTrigger;

	std::vector<Vector3> normals;
	std::vector<Vector3> worldNormals;

	void calculateWorldData();

private:
	virtual bool isCollidingWithSphere(SphereCollider* other) = 0;
	virtual bool isCollidingWithBox(BoxCollider* other) = 0;
	virtual bool isCollidingWithFrustum(FrustumCollider* other) = 0;
	virtual bool isCollidingWithBounds(Bounds other) = 0;
};

#endif