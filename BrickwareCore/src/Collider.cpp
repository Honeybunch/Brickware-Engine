#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore\SphereCollider.hpp"
#include "BrickwareCore\BoxCollider.hpp"
#include "BrickwareCore\FrustumCollider.hpp"
#include "BrickwareCore\Transform.hpp"
#include "BrickwareCore\GameObject.hpp"
#include "BrickwareCore\PhysicsManager.hpp"

#include "BrickwareMath\Bounds.hpp"

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;

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

	Primitive::SetColor(Vector4(1, 0, 0, 1));

	for (unsigned int i = 0; i < normals.size(); i++)
	{
		Vector3 worldNormal = rotationMatrix * normals[i];

		//Renormalize after being rotated
		worldNormals.push_back(Vector3::Normalize(worldNormal));

		if (Debug::Debugging)
			Primitive::DrawLine(center, center + worldNormals[i] * 2);
	}
}

Collider::~Collider()
{
	PhysicsManager::RemoveCollider(this);
}
