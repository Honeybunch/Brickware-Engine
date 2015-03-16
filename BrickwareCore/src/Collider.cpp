#define BRICKWARE_CORE_EXPORTS

#include "SphereCollider.h"
#include "BoxCollider.h"
#include "FrustumCollider.h"
#include "Bounds.h"
#include "Transform.h"
#include "GameObject.h"
#include "PhysicsManager.h"

Collider::Collider()
{
	enabled = true;
	isTrigger = false;
	PhysicsManager::AddCollider(this);
}

bool Collider::isColliding(Collider* other, Collision* collision)
{
	SphereCollider* otherSphere = dynamic_cast<SphereCollider*>(other);
	BoxCollider* otherAxisAlignedBox = dynamic_cast<BoxCollider*>(other);
	FrustumCollider* otherFrustumCollider = dynamic_cast<FrustumCollider*>(other);

	calculateWorldData();
	other->calculateWorldData();

	if (otherSphere)
		return isCollidingWithSphere(otherSphere);
	else if (otherAxisAlignedBox)
		return isCollidingWithBox(otherAxisAlignedBox, collision);
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
	worldNormals.clear();

	Transform* transform = getGameObject()->getTransform();
	center = transform->getPosition();
	Matrix3 rotationMatrix = transform->getRotation().getRotationMatrix();

	for (unsigned int i = 0; i < normals.size(); i++)
	{
		Vector3 worldNormal = rotationMatrix * normals[i];

		//Renormalize after being rotated
		worldNormals.push_back(Vector3::Normalize(worldNormal));
	}	
}

Collider::~Collider()
{
	PhysicsManager::RemoveCollider(this);
}
