#define BRICKWARE_CORE_EXPORTS

#include "SphereCollider.h"
#include "BoxCollider.h"
#include "FrustumCollider.h"
#include "Bounds.h"
#include "Transform.h"
#include "GameObject.h"

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

bool Collider::isColliding(Bounds bounds)
{
	return isCollidingWithBounds(bounds);
}

void Collider::calculateWorldData()
{
	Transform* transform = getGameObject()->getTransform();

	worldNormals.clear();

	//TODO: just use a quaternion to matrix3 conversion instead
	Matrix4 modelMatrix = transform->getModelMatrix();

	for (unsigned int i = 0; i < normals.size(); i++)
	{
		Vector3 worldNormal = modelMatrix * normals[i];

		//Renormalize after being rotated and scaled and whatnot
		worldNormals.push_back(Vector3::Normalize(worldNormal));
	}	
}

Collider::~Collider()
{
}
