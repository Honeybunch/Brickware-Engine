#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/Transform.hpp"
#include "BrickwareCore/GameObject.hpp"
#include "BrickwareCore/Camera.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

#include <iostream>

using namespace Brickware;
using namespace Core;
using namespace Math;

Transform::Transform()
{
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotation = Quaternion::getQuaternionIdentity();
	scale = Vector3(1.0f, 1.0f, 1.0f);

	Update();
}

//Accessors
Vector3 Transform::getPosition(){ return position; }
Vector3 Transform::getEulerRotation(){ return eulerRotation; }
Quaternion Transform::getRotation(){ return rotation; }
//Vector3 Transform::getWorldEulerRotation(){ return worldEulerRotation; }
//Quaternion Transform::getWorldRotation(){ return worldRotation; }
Vector3 Transform::getScale(){ return scale; }

Vector3 Transform::getForward(){ return forward; }
Vector3 Transform::getRight(){ return right; }
Vector3 Transform::getUp(){ return up; }

Matrix4 Transform::getModelMatrix(){ return modelMatrix; }

Matrix4 Transform::getTranslationMatrix(){ return translationMatrix; }
Matrix4 Transform::getRotationMatrix(){ return rotationMatrix; }
Matrix4 Transform::getScaleMatrix(){ return scaleMatrix; }

//Mutators
void Transform::setPosition(Vector3 newPosition)
{ 
	position = newPosition; 
	Update();
}
void Transform::setEulerRotation(Vector3 newEulerRotation){ 
	rotation = Quaternion(newEulerRotation);
	eulerRotation = newEulerRotation;
	Update();
}
/*
void Transform::setWorldEulerRotation(Vector3 newEulerRotation){

	Quaternion worldRotX = Quaternion(Vector3(newEulerRotation[0], 0, 0));
	Quaternion worldRotY = Quaternion(Vector3(0, newEulerRotation[1], 0));
	Quaternion worldRotZ = Quaternion(Vector3(0, 0, newEulerRotation[2]));

	worldRotation = worldRotX * worldRotY * worldRotZ;

	worldEulerRotation = newEulerRotation;
}*/
void Transform::setRotation(Quaternion newRotation)
{ 
	rotation = newRotation; 
	Update();
}
//void Transform::setWorldRotation(Quaternion newRotation){ worldRotation = newRotation; }
void Transform::setScale(Vector3 newScale)
{ 
	scale = newScale; 
	Update();
}

//Public Functions
Component* Transform::Clone(){ return new Transform(*this); }

void Transform::Update()
{
	rotationMatrix = rotation.getRotationMatrix();

	translationMatrix = Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
						      0.0f, 1.0f, 0.0f, 0.0f,
						      0.0f, 0.0f, 1.0f, 0.0f,
						      position.getX(), position.getY(), position.getZ(), 1.0f);

	scaleMatrix = Matrix4(scale.getX(), 0.0f, 0.0f, 0.0f,
						0.0f, scale.getY(), 0.0f, 0.0f,
						0.0f, 0.0f, scale.getZ(), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);

	modelMatrix = (scaleMatrix * rotationMatrix * translationMatrix);

	//Recalculate forward, right and up

	forward = Vector3::Normalize((modelMatrix * Vector3(0, 0, -1) - position));
	right = Vector3::Normalize((modelMatrix * Vector3(1, 0, 0) - position));
	up = Vector3::Normalize((modelMatrix * Vector3(0, 1, 0) - position));
}

void Transform::Render()
{
	MeshRenderer* meshRenderer = this->getGameObject()->getComponent<MeshRenderer>();
	
	if (meshRenderer)
	{
		Graphics::Material* material = meshRenderer->getMaterial();
		Graphics::Material* shadowMaterial = meshRenderer->getShadowMaterial();

		if (material)
		{
			material->setMatrix4("rotationMatrix", rotation.getRotationMatrix());
			material->setMatrix4("modelMatrix", modelMatrix);
		}

		if (shadowMaterial)
		{
			shadowMaterial->setMatrix4("modelMatrix", modelMatrix);
		}
	}
}

Transform::~Transform()
{

}
