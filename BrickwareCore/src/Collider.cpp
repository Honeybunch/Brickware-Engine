#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/SphereCollider.hpp"
#include "BrickwareCore/BoxCollider.hpp"
#include "BrickwareCore/FrustumCollider.hpp"
#include "BrickwareCore/Transform.hpp"
#include "BrickwareCore/GameObject.hpp"
#include "BrickwareCore/PhysicsManager.hpp"

#include "BrickwareMath/Bounds.hpp"

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

bool Collider::isColliding(Ray ray, Vector3* pointOfCollision)
{
	return isCollidingWithRay(ray, pointOfCollision);
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

bool Collider::TestSAT(Collider* other, Collision* collision)
{
	if (this == other)
		return false;

	bool collided = true; 
	float smallestOverlap = std::numeric_limits<float>::max();
	std::vector<Vector3> pointsOfCollision;
	Vector3 possibleMTV;

	//Need MeshRenderer Components 
	MeshRenderer* thisMeshRenderer = this->getGameObject()->getComponent<MeshRenderer>();
	MeshRenderer* otherMeshRenderer = other->getGameObject()->getComponent<MeshRenderer>();

	if (thisMeshRenderer == nullptr)
	{
		std::cout << "Error testing SAT. The called collider did not have an attached MeshRenderer." << std::endl;
		return false;
	}
	else if (otherMeshRenderer == nullptr)
	{
		std::cout << "Error testing SAT. The passed collider did not have an attached MeshRenderer." << std::endl;
		return false;
	}

	//Save object rotations
	Matrix4 thisRotation = this->getGameObject()->getTransform()->getRotationMatrix();
	Matrix4 otherRotation = other->getGameObject()->getTransform()->getRotationMatrix();

	//Store object axes
	std::vector<Vector3> thisAxes = worldNormals;
	std::vector<Vector3> otherAxes = other->worldNormals;

	//Vectors to store projeceted bounds
	Vector2 thisProjectedBounds;
	Vector2 otherProjectedBounds;

	//Test this object's axes
	for (unsigned int i = 0; i < thisAxes.size(); i++)
	{
		Vector3 axis = thisAxes[i];

		thisProjectedBounds = this->getProjectedBounds(axis);
		otherProjectedBounds = other->getProjectedBounds(axis);

		//Check if bounds overlap
		if (thisProjectedBounds[0] < otherProjectedBounds[1] && thisProjectedBounds[1] > otherProjectedBounds[0])
		{
			//Find smallest overlap
			float thisOverlap = otherProjectedBounds[1] - thisProjectedBounds[0];
			float otherOverlap = thisProjectedBounds[1] - otherProjectedBounds[0];

			float smallerOverlap = 0;
			if (thisOverlap < otherOverlap)
				smallerOverlap = thisOverlap;
			else
				smallerOverlap = otherOverlap;

			//If this is the first pass or if we've found a smaller overlap we'll set some new collision data
			if (i == 0 || smallerOverlap < smallestOverlap)
			{
				smallestOverlap = smallerOverlap;
				possibleMTV = (axis * smallestOverlap);
				pointsOfCollision.push_back(possibleMTV);
			}
		}
		else
		{
			//If there is no overlap then the SAT test can end in failure for this object
			collided = false;
			break;
		}
	}

	//Test other object's axes if this object detected a collision
	if (collided)
	{
		for (unsigned int i = 0; i < otherAxes.size(); i++)
		{
			Vector3 axis = otherAxes[i];

			thisProjectedBounds = this->getProjectedBounds(axis);
			otherProjectedBounds = other->getProjectedBounds(axis);

			//Check if bounds overlap
			if (thisProjectedBounds[0] < otherProjectedBounds[1] && thisProjectedBounds[1] > otherProjectedBounds[0])
			{
				//Find smallest overlap
				float thisOverlap = otherProjectedBounds[1] - thisProjectedBounds[0];
				float otherOverlap = thisProjectedBounds[1] - otherProjectedBounds[0];

				float smallerOverlap = 0;
				if (thisOverlap < otherOverlap)
					smallerOverlap = thisOverlap;
				else
					smallerOverlap = otherOverlap;

				//If this is the first pass or if we've found a smaller overlap we'll set some new collision data
				if (smallerOverlap < smallestOverlap)
				{
					smallestOverlap = smallerOverlap;
					possibleMTV = (axis * smallestOverlap);
					pointsOfCollision.push_back(possibleMTV);
				}
			}
			else
			{
				//If there is no overlap then the SAT test can end in failure for this object
				collided = false;
				break;
			}
		}
	}
	
	//Fill collision data if we were given an object to fill
	if (collided && collision != nullptr)
	{
		//If our MTV is pointed into our object, just reverse it
		if (Vector3::Dot(other->center - center, possibleMTV) > 0)
			possibleMTV *= -1;

		collision->setThisObject(getGameObject());
		collision->setThisRigidbody(this->getGameObject()->getComponent<Rigidbody>());
		collision->setThisCollider(this);

		collision->setOtherObject(other->getGameObject());
		collision->setOtherCollider(other);
		collision->setOtherRigidbody(other->getGameObject()->getComponent<Rigidbody>());

		collision->setMTV(possibleMTV);
		collision->setPointsOfCollision(pointsOfCollision);
	}

	if (collided)
		int i = 0;

	return collided;
}

//Return the min and max points of the game object's mesh projected along an axis
Vector2 Collider::getProjectedBounds(Vector3 axis)
{
	MeshRenderer* meshRenderer = getGameObject()->getComponent<MeshRenderer>();
	Transform* transform = getGameObject()->getTransform();
	Vector2 projectedBounds;

	if (meshRenderer == nullptr)
	{
		std::cout << "Tried to getProjectdBounds but there was no attached MeshRenderer. Returning default Vector2" << std::endl;
		return projectedBounds;
	}

	//Get transform data to get the mesh points into the proper frame of reference
	Matrix4 basisMatrix = transform->getModelMatrix();

	std::vector<Vector3> points = meshRenderer->getMesh()->getVerticies();
	
	//Set defaults to be projection of first point
	float projection = Vector3::Dot(axis, basisMatrix * points[0]);

	projectedBounds[0] = projection;
	projectedBounds[1] = projection;

	for (unsigned int i = 1; i < points.size(); i++)
	{
		projection = Vector3::Dot(axis, basisMatrix * points[i]);
		if (projection < projectedBounds[0])
			projectedBounds[0] = projection;
		else if (projection > projectedBounds[1])
			projectedBounds[1] = projection;
	}

	return projectedBounds;
}

Collider::~Collider()
{
	PhysicsManager::RemoveCollider(this);
}
