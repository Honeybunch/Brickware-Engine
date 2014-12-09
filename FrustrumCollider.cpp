#include "FrustrumCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Bounds.h"
#include "Camera.h"

FrustrumCollider::FrustrumCollider(float zNear, float zFar, float FoV, float aspectRatio)
{
	float frustrumNearHeight = 2.0f * zNear * tanf(FoV * 0.5f * (float)(M_PI / 180.0f));
	float frustrumNearWidth = frustrumNearHeight * aspectRatio;

	float frustrumFarHeight = 2.0f * zNear * tanf(FoV * 0.5f * (float)(M_PI / 180.0f));
	float frustrumFarWidth = frustrumFarHeight * aspectRatio;

	nearBottomLeft = new Vector3(frustrumNearWidth / -2, frustrumNearHeight / -2, zNear);
	nearBottomRight = new Vector3(frustrumNearWidth / 2, frustrumNearHeight / -2, zNear);
	nearTopLeft = new Vector3(frustrumNearWidth / -2, frustrumNearHeight / 2, zNear);
	nearTopRight = new Vector3(frustrumNearWidth / 2, frustrumNearHeight / 2, zNear);

	farBottomLeft = new Vector3(frustrumFarWidth / -2, frustrumFarHeight / -2, zFar);
	farBottomRight = new Vector3(frustrumFarWidth / 2, frustrumFarHeight / -2, zFar);
	farTopLeft = new Vector3(frustrumFarWidth / -2, frustrumFarHeight / 2, zFar);
	farTopRight = new Vector3(frustrumFarWidth / 2, frustrumFarHeight / 2, zFar);
}

void FrustrumCollider::Update()
{
	nearBottomLeft = &(getGameObject()->getTransform()->getModelMatrix() * (*nearBottomLeft));
	nearBottomRight = &(getGameObject()->getTransform()->getModelMatrix() * (*nearBottomRight));
	nearTopLeft = &(getGameObject()->getTransform()->getModelMatrix() * (*nearTopLeft));
	nearTopRight = &(getGameObject()->getTransform()->getModelMatrix() * (*nearTopRight));
	
	farBottomLeft = &(getGameObject()->getTransform()->getModelMatrix() * (*farBottomLeft));
	farBottomRight = &(getGameObject()->getTransform()->getModelMatrix() * (*farBottomRight));
	farTopLeft = &(getGameObject()->getTransform()->getModelMatrix() * (*farTopLeft));
	farTopRight = &(getGameObject()->getTransform()->getModelMatrix() * (*farTopRight));
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
	Vector3 nearNorm = Vector3::Normalize(Vector3::Cross((*nearBottomLeft - *nearBottomRight), (*nearBottomRight - *nearTopRight)));
	Vector3 farNorm = Vector3::Normalize(Vector3::Cross((*farBottomLeft - *farBottomRight), (*farBottomRight - *farTopRight)));
	Vector3 leftNorm = Vector3::Normalize(Vector3::Cross((*nearBottomLeft - *farBottomLeft), (*farBottomLeft - *farTopLeft)));
	Vector3 rightNorm = Vector3::Normalize(Vector3::Cross((*nearBottomRight - *farBottomRight), (*farBottomRight - *farTopRight)));
	Vector3 upNorm = Vector3::Normalize(Vector3::Cross((*nearTopLeft - *nearTopRight), (*nearTopRight - *farTopRight)));
	Vector3 downNorm = Vector3::Normalize(Vector3::Cross((*nearBottomLeft - *nearBottomRight), (*nearBottomRight - *farBottomRight)));

	Vector3 normals[] = { nearNorm, farNorm, leftNorm, rightNorm, upNorm, downNorm };

	for (int i = 0; i < 6; i++)
	{
		Vector3 normal = normals[i];

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

		//If the positive and negative verts are in front of a normal than it is not inside of the frustrum
		//Return false
		if (Vector3::Dot(normals[i], positive) >= 0 && Vector3::Dot(normals[i], negative) >= 0)
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
