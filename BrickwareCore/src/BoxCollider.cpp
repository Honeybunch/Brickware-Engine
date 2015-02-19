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

	Vector3 scale = transform->getScale();
	halfSize = Vector3(xSizeHalf * scale[0], ySizeHalf * scale[1], zSizeHalf * scale[2]);

	//We can find three normals along each axis; don't need to find the other three
	//Because they're just opposites of the ones we're storing
	normals.clear();
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
	float radiusThis, radiusOther;
	Matrix3 rot, absRot;

	//Calculate rot matrix expressing the other box in this box's coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			rot[i][j] = Vector3::Dot(worldNormals[i], other->worldNormals[j]);
	
	//Compute the translation vector
	Vector3 translation = other->center - center;
	Vector3 t = other->center - center;
	//Make sure it's in this box's coordinate frame
	translation = Vector3(Vector3::Dot(translation, worldNormals[0]),
						  Vector3::Dot(translation, worldNormals[1]),
						  Vector3::Dot(translation, worldNormals[2]));

	//Calculate absolute value of rot matrix
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			absRot[i][j] = fabsf(rot[i][j]) + std::numeric_limits<float>::epsilon();

	//Test local axes of this box
	for (int i = 0; i < 3; i++)
	{
		radiusThis = halfSize[i];
		radiusOther = other->halfSize[0] * absRot[i][0]
					+ other->halfSize[1] * absRot[i][1]
					+ other->halfSize[2] * absRot[i][2];

		if (fabsf(translation[i]) > radiusThis + radiusOther)
			return false;
	}

	//Test local axes of other box
	for (int i = 0; i < 3; i++)
	{
		radiusThis = halfSize[0] * absRot[0][i]
				   + halfSize[1] * absRot[1][i]
				   + halfSize[2] * absRot[2][i];
		radiusOther = other->halfSize[i];

		if (fabsf((translation[0] * rot[0][i]) + 
				   (translation[1] * rot[1][i]) + 
				   (translation[2] * rot[2][i]))
		> radiusThis + radiusOther)
			return false;
	}

	//------------------------------------------------------------------

	//Test axis ThisX x OtherX
	radiusThis = (		  halfSize[1] * absRot[2][0]) + (		halfSize[2] * absRot[1][0]);
	radiusOther = (other->halfSize[1] * absRot[0][2]) + (other->halfSize[2] * absRot[0][1]);
	if (fabsf((translation[2] * rot[1][0]) - (translation[1] * rot[2][0]))
		> radiusThis + radiusOther)
		return false;

	//Test axis ThisX x OtherY
	radiusThis = (		  halfSize[1] * absRot[2][1]) + (		halfSize[2] * absRot[1][1]);
	radiusOther = (other->halfSize[0] * absRot[0][2]) + (other->halfSize[2] * absRot[0][0]);
	if (fabsf((translation[2] * rot[1][1]) - (translation[1] * rot[2][1]))
	> radiusThis + radiusOther)
		return false;

	//Test axis ThisX x OtherZ
	radiusThis = (		  halfSize[1] * absRot[2][2]) + (		halfSize[2] * absRot[1][2]);
	radiusOther = (other->halfSize[0] * absRot[0][1]) + (other->halfSize[1] * absRot[0][0]);
	if (fabsf((translation[2] * rot[1][2]) - (translation[1] * rot[2][2]))
		> radiusThis + radiusOther)
		return false;

	//------------------------------------------------------------------

	//Test axis ThisY x OtherX
	radiusThis = (		  halfSize[0] * absRot[2][0]) + (		halfSize[2] * absRot[0][0]);
	radiusOther = (other->halfSize[1] * absRot[1][2]) + (other->halfSize[2] * absRot[1][1]);
	if (fabsf((translation[0] * rot[2][0]) - (translation[2] * rot[0][0]))
	> radiusThis + radiusOther)
		return false;

	//Test axis ThisY x OtherY
	radiusThis = (		  halfSize[0] * absRot[2][1]) + (		halfSize[2] * absRot[0][1]);
	radiusOther = (other->halfSize[0] * absRot[1][2]) + (other->halfSize[2] * absRot[1][0]);
	if (fabsf((translation[0] * rot[2][1]) - (translation[2] * rot[0][1]))
	> radiusThis + radiusOther)
		return false;

	//Test axis ThisY x OtherZ
	radiusThis = (		  halfSize[0] * absRot[2][2]) + (		halfSize[2] * absRot[0][2]);
	radiusOther = (other->halfSize[0] * absRot[1][1]) + (other->halfSize[1] * absRot[1][0]);
	if (fabsf((translation[0] * rot[2][2]) - (translation[2] * rot[0][2]))
		> radiusThis + radiusOther)
		return false;

	//------------------------------------------------------------------

	//Test axis ThisZ x OtherX
	radiusThis = (		  halfSize[0] * absRot[1][0]) + (		halfSize[1] * absRot[0][0]);
	radiusOther = (other->halfSize[1] * absRot[2][2]) + (other->halfSize[2] * absRot[2][1]);
	if (fabsf((translation[1] * rot[0][0]) - (translation[0] * rot[1][0]))
	> radiusThis + radiusOther)
		return false;

	//Test axis ThisZ x OtherY
	radiusThis = (		  halfSize[0] * absRot[1][1]) + (		halfSize[1] * absRot[0][1]);
	radiusOther = (other->halfSize[0] * absRot[2][2]) + (other->halfSize[2] * absRot[2][0]);
	if (fabsf((translation[1] * rot[0][1]) - (translation[0] * rot[1][1]))
	> radiusThis + radiusOther)
		return false;

	//Test axis ThisZ x OtherZ
	radiusThis = (		  halfSize[0] * absRot[1][2]) + (		halfSize[1] * absRot[0][2]);
	radiusOther = (other->halfSize[0] * absRot[2][1]) + (other->halfSize[1] * absRot[2][0]);
	if (fabsf((translation[1] * rot[0][2]) - (translation[0] * rot[1][2]))
		> radiusThis + radiusOther)
		return false;
		
  	return true;
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
