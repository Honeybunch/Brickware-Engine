#define BRICKWARE_CORE_EXPORTS

#include "GameObject.h"
#include "Bounds.h"
#include "SphereCollider.h"
#include "FrustumCollider.h"
#include "BoxCollider.h"
#include "MeshRenderer.h"

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
	size = Vector3(size[0] * scale[0], size[1] * scale[1], size[2] * scale[2]);
	halfSize = Vector3(xSizeHalf * scale[0], ySizeHalf * scale[1], zSizeHalf * scale[2]);

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

//TODO: Implement
bool BoxCollider::isCollidingWithBox(BoxCollider* other, Collision* collision)
{
	float radiusThis, radiusOther;
	float projectedTranslation;
	Matrix3 rot, absRot;
	
	//Things we may end up returning
	Vector3 possibleMTV = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	std::vector<Vector3> pointsOfCollision;

	//Calculate rot matrix expressing the other box in this box's coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			rot[i][j] = Vector3::Dot(worldNormals[i], other->worldNormals[j]);
	
	//Compute the translation vector
	Vector3 translation = other->center - center;
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
		projectedTranslation = fabsf(translation[i]);

		radiusThis = halfSize[i];
		radiusOther = other->halfSize[0] * absRot[i][0]
					+ other->halfSize[1] * absRot[i][1]
					+ other->halfSize[2] * absRot[i][2];

		if (projectedTranslation > radiusThis + radiusOther)
			return false;
		else
		{
			float overlap = (radiusThis + radiusOther) - projectedTranslation;
			float distanceToPoint = radiusThis - overlap;

			Vector3 axis = worldNormals[i];
			axis = Vector3::Normalize(axis);

			if (axis.getMagnitude() > 0)
			{
				Vector3 translationVector = axis * overlap;
				Vector3 pointOfCollision = center + (axis * distanceToPoint);

				pointsOfCollision.push_back(pointOfCollision);

				if (translationVector < possibleMTV)
					possibleMTV = translationVector;
			}
		}
	}

	//Test local axes of other box
	for (int i = 0; i < 3; i++)
	{
		projectedTranslation = fabsf((translation[0] * rot[0][i]) +
									 (translation[1] * rot[1][i]) +
									 (translation[2] * rot[2][i]));

		radiusThis = halfSize[0] * absRot[0][i]
				   + halfSize[1] * absRot[1][i]
				   + halfSize[2] * absRot[2][i];
		radiusOther = other->halfSize[i];

		if (projectedTranslation > radiusThis + radiusOther)
			return false;
		else
		{
			float overlap = (radiusThis + radiusOther) - projectedTranslation;
			float distanceToPoint = radiusThis - overlap;

			Vector3 axis = other->worldNormals[i];
			axis = Vector3::Normalize(axis);

			if (axis.getMagnitude() > 0)
			{
				Vector3 translationVector = axis * overlap;
				Vector3 pointOfCollision = center + (axis * distanceToPoint);

				pointsOfCollision.push_back(pointOfCollision);

				if (translationVector < possibleMTV)
					possibleMTV = translationVector;
			}
		}
	}

	//------------------------------------------------------------------

	//Test axis ThisX x OtherX
	projectedTranslation = fabsf((translation[2] * rot[1][0]) - (translation[1] * rot[2][0]));
	radiusThis = (		  halfSize[1] * absRot[2][0]) + (		halfSize[2] * absRot[1][0]);
	radiusOther = (other->halfSize[1] * absRot[0][2]) + (other->halfSize[2] * absRot[0][1]);
	if (projectedTranslation > radiusThis + radiusOther)
		return false;
	else
	{
		float overlap = (radiusThis + radiusOther) - projectedTranslation;
		float distanceToPoint = radiusThis - overlap;

		Vector3 axis = Vector3::Cross(worldNormals[0], other->worldNormals[0]);
		axis = Vector3::Normalize(axis);

		if (axis.getMagnitude() > 0)
		{
			Vector3 translationVector = axis * overlap;
			Vector3 pointOfCollision = center + (axis * distanceToPoint);

			pointsOfCollision.push_back(pointOfCollision);

			if (translationVector < possibleMTV)
				possibleMTV = translationVector;
		}
	}

	//Test axis ThisX x OtherY
	projectedTranslation = fabsf((translation[2] * rot[1][1]) - (translation[1] * rot[2][1]));
	radiusThis = (		  halfSize[1] * absRot[2][1]) + (		halfSize[2] * absRot[1][1]);
	radiusOther = (other->halfSize[0] * absRot[0][2]) + (other->halfSize[2] * absRot[0][0]);
	if (projectedTranslation > radiusThis + radiusOther)
		return false;
	else
	{
		float overlap = (radiusThis + radiusOther) - projectedTranslation;
		float distanceToPoint = radiusThis - overlap;

		Vector3 axis = Vector3::Cross(worldNormals[0], other->worldNormals[1]);
		axis = Vector3::Normalize(axis);

		if (axis.getMagnitude() > 0)
		{
			Vector3 translationVector = axis * overlap;
			Vector3 pointOfCollision = center + (axis * distanceToPoint);

			pointsOfCollision.push_back(pointOfCollision);

			if (translationVector < possibleMTV)
				possibleMTV = translationVector;
		}
	}

	//Test axis ThisX x OtherZ
	projectedTranslation = fabsf((translation[2] * rot[1][2]) - (translation[1] * rot[2][2]));
	radiusThis = (		  halfSize[1] * absRot[2][2]) + (		halfSize[2] * absRot[1][2]);
	radiusOther = (other->halfSize[0] * absRot[0][1]) + (other->halfSize[1] * absRot[0][0]);
	if (projectedTranslation > radiusThis + radiusOther)
 		return false;
	else
	{
		float overlap = (radiusThis + radiusOther) - projectedTranslation;
		float distanceToPoint = radiusThis - overlap;

		Vector3 axis = Vector3::Cross(worldNormals[0], other->worldNormals[2]);
		axis = Vector3::Normalize(axis);

		if (axis.getMagnitude() > 0)
		{
			Vector3 translationVector = axis * overlap;
			Vector3 pointOfCollision = center + (axis * distanceToPoint);

			pointsOfCollision.push_back(pointOfCollision);

			if (translationVector < possibleMTV)
				possibleMTV = translationVector;
		}
	}

	//------------------------------------------------------------------

	//Test axis ThisY x OtherX
	projectedTranslation = fabsf((translation[0] * rot[2][0]) - (translation[2] * rot[0][0]));
	radiusThis = (		  halfSize[0] * absRot[2][0]) + (		halfSize[2] * absRot[0][0]);
	radiusOther = (other->halfSize[1] * absRot[1][2]) + (other->halfSize[2] * absRot[1][1]);
	if (projectedTranslation > radiusThis + radiusOther)
		return false;
	else
	{
		float overlap = (radiusThis + radiusOther) - projectedTranslation;
		float distanceToPoint = radiusThis - overlap;

		Vector3 axis = Vector3::Cross(worldNormals[1], other->worldNormals[0]);
		axis = Vector3::Normalize(axis);

		if (axis.getMagnitude() > 0)
		{
			Vector3 translationVector = axis * overlap;
			Vector3 pointOfCollision = center + (axis * distanceToPoint);

			pointsOfCollision.push_back(pointOfCollision);

			if (translationVector < possibleMTV)
				possibleMTV = translationVector;
		}
	}

	//Test axis ThisY x OtherY
	projectedTranslation = fabsf((translation[0] * rot[2][1]) - (translation[2] * rot[0][1]));
	radiusThis = (		  halfSize[0] * absRot[2][1]) + (		halfSize[2] * absRot[0][1]);
	radiusOther = (other->halfSize[0] * absRot[1][2]) + (other->halfSize[2] * absRot[1][0]);
	if (projectedTranslation > radiusThis + radiusOther)
		return false;
	else
	{
		float overlap = (radiusThis + radiusOther) - projectedTranslation;
		float distanceToPoint = radiusThis - overlap;

		Vector3 axis = Vector3::Cross(worldNormals[1], other->worldNormals[1]);
		axis = Vector3::Normalize(axis);

		if (axis.getMagnitude() > 0)
		{
			Vector3 translationVector = axis * overlap;
			Vector3 pointOfCollision = center + (axis * distanceToPoint);

			pointsOfCollision.push_back(pointOfCollision);

			if (translationVector < possibleMTV)
				possibleMTV = translationVector;
		}
	}

	//Test axis ThisY x OtherZ
	projectedTranslation = fabsf((translation[0] * rot[2][2]) - (translation[2] * rot[0][2]));
	radiusThis = (		  halfSize[0] * absRot[2][2]) + (		halfSize[2] * absRot[0][2]);
	radiusOther = (other->halfSize[0] * absRot[1][1]) + (other->halfSize[1] * absRot[1][0]);
	if (projectedTranslation > radiusThis + radiusOther)
		return false;
	else
	{
		float overlap = (radiusThis + radiusOther) - projectedTranslation;
		float distanceToPoint = radiusThis - overlap;

		Vector3 axis = Vector3::Cross(worldNormals[1], other->worldNormals[2]);
		axis = Vector3::Normalize(axis);

		if (axis.getMagnitude() > 0)
		{
			Vector3 translationVector = axis * overlap;
			Vector3 pointOfCollision = center + (axis * distanceToPoint);

			pointsOfCollision.push_back(pointOfCollision);

			if (translationVector < possibleMTV)
				possibleMTV = translationVector;
		}
	}

	//------------------------------------------------------------------

	//Test axis ThisZ x OtherX
	projectedTranslation = fabsf((translation[1] * rot[0][0]) - (translation[0] * rot[1][0]));
	radiusThis = (		  halfSize[0] * absRot[1][0]) + (		halfSize[1] * absRot[0][0]);
	radiusOther = (other->halfSize[1] * absRot[2][2]) + (other->halfSize[2] * absRot[2][1]);
	if (projectedTranslation > radiusThis + radiusOther)
		return false;
	else
	{
		float overlap = (radiusThis + radiusOther) - projectedTranslation;
		float distanceToPoint = radiusThis - overlap;

		Vector3 axis = Vector3::Cross(worldNormals[2], other->worldNormals[0]);
		axis = Vector3::Normalize(axis);

		if (axis.getMagnitude() > 0)
		{
			Vector3 translationVector = axis * overlap;
			Vector3 pointOfCollision = center + (axis * distanceToPoint);

			pointsOfCollision.push_back(pointOfCollision);

			if (translationVector < possibleMTV)
				possibleMTV = translationVector;
		}
	}

	//Test axis ThisZ x OtherY
	projectedTranslation = fabsf((translation[1] * rot[0][1]) - (translation[0] * rot[1][1]));
	radiusThis = (		  halfSize[0] * absRot[1][1]) + (		halfSize[1] * absRot[0][1]);
	radiusOther = (other->halfSize[0] * absRot[2][2]) + (other->halfSize[2] * absRot[2][0]);
	if (projectedTranslation > radiusThis + radiusOther)
		return false;
	else
	{
		float overlap = (radiusThis + radiusOther) - projectedTranslation;
		float distanceToPoint = radiusThis - overlap;

		Vector3 axis = Vector3::Cross(worldNormals[2], other->worldNormals[1]);
		axis = Vector3::Normalize(axis);

		if (axis.getMagnitude() > 0)
		{
			Vector3 translationVector = axis * overlap;
			Vector3 pointOfCollision = center + (axis * distanceToPoint);

			pointsOfCollision.push_back(pointOfCollision);

			if (translationVector < possibleMTV)
				possibleMTV = translationVector;
		}
	}

	//Test axis ThisZ x OtherZ
	projectedTranslation = fabsf((translation[1] * rot[0][2]) - (translation[0] * rot[1][2]));
	radiusThis = (		  halfSize[0] * absRot[1][2]) + (		halfSize[1] * absRot[0][2]);
	radiusOther = (other->halfSize[0] * absRot[2][1]) + (other->halfSize[1] * absRot[2][0]);
	if (projectedTranslation > radiusThis + radiusOther)
		return false;
	else
	{
		float overlap = (radiusThis + radiusOther) - projectedTranslation;
		float distanceToPoint = radiusThis - overlap;

		Vector3 axis = Vector3::Cross(worldNormals[2], other->worldNormals[2]);
		axis = Vector3::Normalize(axis);

		if (axis.getMagnitude() > 0)
		{
			Vector3 translationVector = axis * overlap;
			Vector3 pointOfCollision = center + (axis * distanceToPoint);

			pointsOfCollision.push_back(pointOfCollision);

			if (translationVector < possibleMTV)
				possibleMTV = translationVector;
		}
	}
		
	//Fill collision data if we were given an object to fill
	if (collision)
	{
		//If our MTV is pointed into our object, just reverse it
		if (Vector3::Dot(other->center - center, possibleMTV) > 0)
			possibleMTV *= -1;

		collision->setThisRigidbody(this->getGameObject()->getComponent<Rigidbody>());
		collision->setThisCollider(this);
		collision->setOtherCollider(other);
		collision->setOtherRigidbody(other->getGameObject()->getComponent<Rigidbody>());
		collision->setMTV(possibleMTV);
		collision->setPointsOfCollision(pointsOfCollision);
	}

  	return true;
}

//TODO: Refactor to mesh collision
bool BoxCollider::isCollidingWithFrustum(FrustumCollider* other)
{
	return other->isColliding(this, NULL);
}

//Don't really care yet
bool BoxCollider::isCollidingWithBounds(Bounds other)
{
	return false;
}

BoxCollider::~BoxCollider()
{
}
