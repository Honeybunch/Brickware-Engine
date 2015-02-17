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

		//Store halfwidths
		size = Vector3(xSize, ySize, zSize);
	}

	//Otherwise we can just assume a size of 1,1,1
	else
	{
		size = Vector3(1,1,1);
	}

	//We store half size because it's more useful in SAT calculations
	float xSizeHalf = size.getX()/2.0f;
	float ySizeHalf = size.getY()/2.0f;
	float zSizeHalf = size.getZ()/2.0f;

	halfSize = Vector3(xSizeHalf, ySizeHalf, zSizeHalf);

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
	calculateWorldData();

	//Calculate rotation matrix expressing the other box in this box's coordinate frame

	//Compute the translation vector
	Vector3 translation = other->center - center;
	//Make sure it's in this box's coordinate frame
	translation = Vector3(Vector3::Dot(translation, normals[0]),
						  Vector3::Dot(translation, normals[2]),
						  Vector3::Dot(translation, normals[2]));

	//Calculate absolute value of rotation matrix



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
