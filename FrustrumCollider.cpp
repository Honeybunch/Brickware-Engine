#include "FrustrumCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Bounds.h"
#include "Camera.h"

#include "GameObject.h"

FrustrumCollider::FrustrumCollider(float zNear, float zFar, float FoV, float aspectRatio)
{
	this->zNear = zNear;
	this->zFar = zFar;

	frustrumNearHeight = 2.0f * zNear * tanf((FoV/2.0f) * (float)(M_PI / 180.0f));
	frustrumNearWidth = frustrumNearHeight * aspectRatio;

	frustrumFarHeight = 2.0f * zFar * tanf((FoV/2.0f) * (float)(M_PI / 180.0f));
	frustrumFarWidth = frustrumFarHeight * aspectRatio;
}

void FrustrumCollider::Update()
{
	nearBottomLeft = getGameObject()->getTransform()->getModelMatrix() * Vector3(frustrumNearWidth / -2, frustrumNearHeight / -2, zNear);
	nearBottomRight = getGameObject()->getTransform()->getModelMatrix() * Vector3(frustrumNearWidth / 2, frustrumNearHeight / -2, zNear);
	nearTopLeft = getGameObject()->getTransform()->getModelMatrix() * Vector3(frustrumNearWidth / -2, frustrumNearHeight / 2, zNear);
	nearTopRight = getGameObject()->getTransform()->getModelMatrix() * Vector3(frustrumNearWidth / 2, frustrumNearHeight / 2, zNear);
	
	farBottomLeft = getGameObject()->getTransform()->getModelMatrix() * Vector3(frustrumFarWidth / -2, frustrumFarHeight / -2, zFar);
	farBottomRight = getGameObject()->getTransform()->getModelMatrix() * Vector3(frustrumFarWidth / 2, frustrumFarHeight / -2, zFar);
	farTopLeft = getGameObject()->getTransform()->getModelMatrix() * Vector3(frustrumFarWidth / -2, frustrumFarHeight / 2, zFar);
	farTopRight = getGameObject()->getTransform()->getModelMatrix() * Vector3(frustrumFarWidth / 2, frustrumFarHeight / 2, zFar);						 
}

void FrustrumCollider::Render()
{
	GameObject* topLeft = new GameObject();
	topLeft->getTransform()->setPosition(&farTopRight);

	Shape sphere(PrimitiveType::SPHERE, 10, 10, 3.0f);
	Mesh sphereMesh(getGameObject()->getComponent<Material>()->getShaderProgram(), sphere, "Textures/stoneTexture.png");
	MeshRenderer renderer(&sphereMesh);

	topLeft->addComponent(&renderer);
	topLeft->addComponent(getGameObject()->getComponent<Material>());

	topLeft->Start();
	topLeft->Update();
	topLeft->OnRender();
}

//We don't really care about this
bool FrustrumCollider::isCollidingWithSphere(SphereCollider* other)
{
	return false;
}

//We don't really care about this
bool FrustrumCollider::isCollidingWithBox(BoxCollider* other)
{
	return false;
}

//TODO: Optimize
bool FrustrumCollider::isCollidingWithBounds(Bounds* other)
{	
	//Get the bounds of the box in a form that easily allows operators
	Vector3 min = other->getMinBound();
	Vector3 max = other->getMaxBound();

	//Derive the 6 normals of the 6 planes of the frustrum
	Vector3 nearNorm = Vector3::Normalize(Vector3::Cross((nearBottomRight - nearBottomLeft), (nearBottomLeft - nearTopLeft)));
	Vector3 farNorm = Vector3::Normalize(Vector3::Cross((farTopLeft - farBottomLeft), (farBottomLeft - farBottomRight)));
	Vector3 leftNorm = Vector3::Normalize(Vector3::Cross((farTopLeft - farBottomLeft), (farBottomLeft - nearBottomLeft)));
	Vector3 rightNorm = Vector3::Normalize(Vector3::Cross((nearTopRight - nearBottomRight), (nearBottomRight - farBottomRight)));
	Vector3 upNorm = Vector3::Normalize(Vector3::Cross((farTopRight - nearTopRight), (nearTopRight - nearTopLeft)));
	Vector3 downNorm = Vector3::Normalize(Vector3::Cross((nearBottomRight - farBottomRight), (farBottomRight - farBottomLeft)));

	Vector3 normals[] = { nearNorm, farNorm, leftNorm, rightNorm, upNorm, downNorm };
	Vector3 points[] = {nearBottomLeft, farBottomLeft, nearBottomLeft, nearBottomRight, nearTopLeft, nearBottomLeft};

	for (int i = 0; i < 6; i++)
	{
		Vector3 normal = normals[i];
		Vector3 point = points[i];

		//Determine the closest and furthest points from this normal
		Vector3 positive(min);
		Vector3 negative(max);

		if (normal.getX() >= 0)
		{
			positive.setX(max.getX());
			negative.setX(min.getX());
		}
		if (normal.getY() >= 0)
		{
			positive.setY(max.getY());
			negative.setY(min.getY());
		}
		if (normal.getZ() >= 0)
		{
			positive.setZ(max.getZ());
			negative.setZ(min.getZ());
		}

		Vector3 positiveDistanceToCenter = positive - point;
		Vector3 negativeDistanceToCenter = negative - point;

		//If the positive and negative verts are in front of a normal than it is not inside of the frustrum
		//Return false
		if (Vector3::Dot(normals[i], positiveDistanceToCenter) >= 0 && Vector3::Dot(normals[i], negativeDistanceToCenter) >= 0)
			return false;
		else
			continue;
	}

	//If we check against every plane and it's never completely in front of a plane than it has intersected at some point 
	//And we can return a collission
	return true;
}

//We don't really care about this 
bool FrustrumCollider::isCollidingWithFrustrum(FrustrumCollider* other)
{
	return false;
}


FrustrumCollider::~FrustrumCollider()
{
}
