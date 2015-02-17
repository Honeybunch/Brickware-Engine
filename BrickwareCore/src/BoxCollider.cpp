#define BRICKWARE_CORE_EXPORTS

#include "GameObject.h"
#include "Bounds.h"
#include "SphereCollider.h"
#include "FrustumCollider.h"
#include "BoxCollider.h"
#include "MeshRenderer.h"

BoxCollider::BoxCollider()
{
	
}

Component* BoxCollider::Clone(){ return new BoxCollider(*(this)); }

void BoxCollider::Start()
{
	//Calculate the points of the box and the normals of the faces
	center = getGameObject()->getTransform()->getPosition();

	//if we have a mesh we will base our size off of the mesh's bounds
	MeshRenderer* meshRenderer = getGameObject()->getComponent<MeshRenderer>();

	if (meshRenderer)
	{
		Bounds bounds = meshRenderer->getBounds();

		Vector3 boundMin = bounds.getMinBound();
		Vector3 boundMax = bounds.getMaxBound();

		float xSize = boundMax.getX() - boundMin.getX();
		float ySize = boundMax.getY() - boundMin.getY();
		float zSize = boundMax.getZ() - boundMin.getZ();

		size = Vector3(xSize, ySize, zSize);
	}

	//Otherwise we can just assume a size of 1,1,1
	else
	{
		size = Vector3(1, 1, 1);
	}

	//Now we can calculate every point and every normal based off of this
	float xSizeHalf = size.getX()/2.0f;
	float ySizeHalf = size.getY()/2.0f;
	float zSizeHalf = size.getZ()/2.0f;

	//x+ is right
	//y+ is up
	//z+ is behind
	Vector3 topLeftForward =	 Vector3(-xSizeHalf, ySizeHalf, -zSizeHalf);
	Vector3 topRightForward =	 Vector3(xSizeHalf, ySizeHalf, -zSizeHalf);
	Vector3 topLeftBackward =	 Vector3(-xSizeHalf, ySizeHalf, zSizeHalf);
	Vector3 topRightBackward =	 Vector3(xSizeHalf, ySizeHalf, zSizeHalf);
	Vector3 bottomLeftForward =	 Vector3(-xSizeHalf, -ySizeHalf, -zSizeHalf);
	Vector3 bottomRightForward = Vector3(xSizeHalf, -ySizeHalf, -zSizeHalf);
	Vector3 bottomLeftBackward = Vector3(-xSizeHalf, -ySizeHalf, zSizeHalf);
	Vector3 bottomRighBackward = Vector3(xSizeHalf, -ySizeHalf, zSizeHalf);

	//Add these all to our vector
	points.push_back(topLeftForward);
	points.push_back(topRightForward);
	points.push_back(topLeftBackward);
	points.push_back(topRightBackward);
	points.push_back(bottomLeftForward);
	points.push_back(bottomRightForward);
	points.push_back(bottomLeftBackward);
	points.push_back(bottomRighBackward);

	//We can find three normals along each axis; don't need to find the other three
	//Because they're just opposites of the ones we're storing
	normals.push_back(Vector3(1, 0, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 0, -1));
}

//Accessors and Mutators
Vector3 BoxCollider::getCenter(){ return center; }
Vector3 BoxCollider::getSize(){ return size; }

bool BoxCollider::isCollidingWithSphere(SphereCollider* other)
{
	return false;
}

//TODO: Implement
bool BoxCollider::isCollidingWithBox(BoxCollider* other)
{
	return false;
}

//TODO: Refactor to mesh collision
bool BoxCollider::isCollidingWithFrustum(FrustumCollider* other)
{
	return other->isColliding(this);
}

//Don't really care yet
bool BoxCollider::isCollidingWithBounds(Bounds other)
{
	return false;
}

BoxCollider::~BoxCollider()
{
}
