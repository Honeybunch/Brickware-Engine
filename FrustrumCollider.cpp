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
	Transform* transform = getGameObject()->getTransform();
	Vector3 position = *(transform->getPosition());
	Vector3 forward = transform->getForward();
	Vector3 right = transform->getRight();
	Vector3 up = transform->getUp();

	Vector3 farCenter = position + (forward * zFar);
	Vector3 nearCenter = position + (forward * zNear);

	farTopLeft = farCenter + (up * (frustrumFarHeight / 2)) - (right * (frustrumFarWidth / 2));
	farTopRight = farCenter + (up * (frustrumFarHeight / 2)) + (right * (frustrumFarWidth / 2));
	farBottomLeft = farCenter - (up * (frustrumFarHeight / 2)) - (right * (frustrumFarWidth / 2));
	farBottomRight = farCenter - (up * (frustrumFarHeight / 2)) + (right * (frustrumFarWidth / 2));

	nearTopLeft = nearCenter + (up * (frustrumNearHeight / 2)) - (right * (frustrumNearWidth / 2));
	nearTopRight = nearCenter + (up * (frustrumNearHeight / 2)) + (right * (frustrumNearWidth / 2));
	nearBottomLeft = nearCenter - (up * (frustrumNearHeight / 2)) - (right * (frustrumNearWidth / 2));
	nearBottomRight = nearCenter - (up * (frustrumNearHeight / 2)) + (right * (frustrumNearWidth / 2));
}

void FrustrumCollider::Render()
{
	//Maybe one day I'll render the collider

	/*
	Transform* transform = getGameObject()->getTransform();
	Vector3 position = *(transform->getPosition());
	Vector3 forward = transform->getForward();

	GameObject* topLeft = new GameObject();
	topLeft->getTransform()->setPosition(&nearTopRight);
	
	Shape sphere(PrimitiveType::SPHERE, 10, 10, 0.01f);
	Mesh sphereMesh(getGameObject()->getComponent<Material>()->getShaderProgram(), sphere, "Textures/stoneTexture.png");
	MeshRenderer renderer(&sphereMesh);
	
	topLeft->addComponent(&renderer);
	topLeft->addComponent(getGameObject()->getComponent<Material>());
	
	topLeft->Start();
	//topLeft->Update();
	topLeft->OnRender();
	*/
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
	Vector3 center = other->getCenter();

	Vector3 position = *getGameObject()->getTransform()->getPosition();

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
		//Get normal and point that describe plane
		Vector3 normal = normals[i];
		Vector3 point = points[i];

		float d = Vector3::Dot(normal, point) * -1.0f;

		//Get most positive vector along the normal
		Vector3 positive = other->getPositive(normal);

		float dist = d + Vector3::Dot(normal, positive);

		if (dist < 0)
			return false;

		/*
		//Get difference between box point and plane point
		Vector3 difference = Vector3::Normalize(positive - point);

		//If dot product between the difference and the normal is less than 0, the box is behind the plane
		float distance = Vector3::Dot(normal, difference);
		if (distance < 0)
			return false;
			*/
	}

	//If we check against every plane and it's never completely in front of a plane than it has intersected at some point 
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
