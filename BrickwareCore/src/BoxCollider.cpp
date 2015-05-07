#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/GameObject.hpp"
#include "BrickwareCore/SphereCollider.hpp"
#include "BrickwareCore/FrustumCollider.hpp"
#include "BrickwareCore/BoxCollider.hpp"
#include "BrickwareCore/MeshRenderer.hpp"

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;

BoxCollider::BoxCollider() : Collider()
{
}

BoxCollider::BoxCollider(BoxCollider& other) : Collider()
{
}

Component* BoxCollider::Clone(){ return new BoxCollider(*(this)); }

void BoxCollider::Start()
{
	//Calculate the points of the box and the normals of the faces
	Transform* transform = getGameObject()->getTransform();
	center = transform->getPosition();

	//if we have a mesh we will base our size off of the mesh's bounds
	MeshRenderer* meshRenderer = getGameObject()->getComponent<MeshRenderer>();

	if (meshRenderer)
	{
		Bounds bounds = meshRenderer->getBounds();

		float xSize = bounds.getXWidth();
		float ySize = bounds.getYWidth();
		float zSize = bounds.getZWidth();

		//Store halfwidths
		size = Math::Vector3(xSize, ySize, zSize);
	}

	//Otherwise we can just assume a size of 1,1,1
	else
	{
		size = Math::Vector3(1,1,1);
	}

	//We store half size because it's more useful in SAT calculations
	float xSizeHalf = size.getX()/2.0f;
	float ySizeHalf = size.getY()/2.0f;
	float zSizeHalf = size.getZ()/2.0f;

	Math::Vector3 scale = transform->getScale();
	size = Math::Vector3(size[0] * scale[0], size[1] * scale[1], size[2] * scale[2]);
	halfSize = Math::Vector3(xSizeHalf * scale[0], ySizeHalf * scale[1], zSizeHalf * scale[2]);

	//We can find three normals along each axis; don't need to find the other three
	//Because they're just opposites of the ones we're storing
	normals.clear();
	normals.push_back(Vector3(1, 0, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 0, 1));
}

#ifdef _DEBUG
void BoxCollider::DebugDraw()
{
	Transform* transform = getGameObject()->getTransform();
	Camera* camera = Camera::GetActiveCamera();

	Vector3 position = transform->getPosition();

	//Gotta get that world matrix (don't need scale)
	Matrix4 rotationMatrix = transform->getRotation().getRotationMatrix();

	Matrix4 translationMatrix(1, 0, 0, 0,
							  0, 1, 0, 0,
							  0, 0, 1, 0,
							  position[0], position[1], position[2], 1);

	Matrix4 worldMatrix = rotationMatrix * translationMatrix;

	//Get world points
	Vector3 topLeftForward = worldMatrix *Vector3(-halfSize[0], halfSize[1], -halfSize[2]);
	Vector3 topRightForward = worldMatrix *Vector3(halfSize[0], halfSize[1], -halfSize[2]);
	Vector3 topLeftBackward = worldMatrix *Vector3(-halfSize[0], halfSize[1], halfSize[2]);
	Vector3 topRightBackward = worldMatrix *Vector3(halfSize[0], halfSize[1], halfSize[2]);

	Vector3 bottomLeftForward = worldMatrix *Vector3(-halfSize[0], -halfSize[1], -halfSize[2]);
	Vector3 bottomRightForward = worldMatrix *Vector3(halfSize[0], -halfSize[1], -halfSize[2]);
	Vector3 bottomLeftBackward = worldMatrix *Vector3(-halfSize[0], -halfSize[1], halfSize[2]);
	Vector3 bottomRightBackward = worldMatrix *Vector3(halfSize[0], -halfSize[1], halfSize[2]);

	//Draw!
	Primitive::SetLineWidth(2.0f);
	Primitive::SetColor(Vector4(0, 0.7f, 0, 1));

	Primitive::DrawLine(topLeftBackward, topRightBackward);
	Primitive::DrawLine(topLeftBackward, bottomLeftBackward);
	Primitive::DrawLine(topRightBackward, bottomRightBackward);
	Primitive::DrawLine(bottomLeftBackward, bottomRightBackward);

	Primitive::DrawLine(topLeftForward, topRightForward);
	Primitive::DrawLine(topLeftForward, bottomLeftForward);
	Primitive::DrawLine(topRightForward, bottomRightForward);
	Primitive::DrawLine(bottomLeftForward, bottomRightForward);

	Primitive::DrawLine(topLeftBackward, topLeftForward);
	Primitive::DrawLine(topRightBackward, topRightForward);
	Primitive::DrawLine(bottomLeftBackward, bottomLeftForward);
	Primitive::DrawLine(bottomRightBackward, bottomRightForward);
}
#endif

//Accessors and Mutators
Vector3 BoxCollider::getCenter(){ return center; }
Vector3 BoxCollider::getSize(){ return size; }

bool BoxCollider::isCollidingWithSphere(SphereCollider* other)
{
	return false;
}

bool BoxCollider::isCollidingWithBox(BoxCollider* other, Collision* collision)
{
	return TestSAT(other, collision);
}

//TODO: Refactor to mesh collision
bool BoxCollider::isCollidingWithFrustum(FrustumCollider* other)
{
	return other->isColliding(this, nullptr);
}

//Don't really care yet
bool BoxCollider::isCollidingWithBounds(Bounds other)
{
	return false;
}

//Don't care yet
bool BoxCollider::isCollidingWithRay(Ray other, Vector3* pointOfCollision)
{
	return false;
}

BoxCollider::~BoxCollider()
{
}
