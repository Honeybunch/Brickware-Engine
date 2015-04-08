#define BRICKWARE_CORE_EXPORTS

#include "Transform.h"
#include "GameObject.h"
#include "Camera.h"

#include <iostream>

using namespace Brickware;
using namespace Core;
using namespace Math;

Transform::Transform()
{
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotation = Quaternion::getQuaternionIdentity();
	scale = Vector3(1.0f, 1.0f, 1.0f);
}

//Accessors
Vector3 Transform::getPosition(){ return position; }
Vector3 Transform::getEulerRotation(){ return eulerRotation; }
Quaternion Transform::getRotation(){ return rotation; }
Vector3 Transform::getScale(){ return scale; }

Vector3 Transform::getForward(){ return forward; }
Vector3 Transform::getRight(){ return right; }
Vector3 Transform::getUp(){ return up; }

Matrix4 Transform::getModelMatrix(){ return modelMatrix; }

//Mutators
void Transform::setPosition(Vector3 newPosition){ position = newPosition; }
void Transform::setEulerRotation(Vector3 newEulerRotation){ 
	rotation = Quaternion(newEulerRotation);
	eulerRotation = newEulerRotation;
}
void Transform::setRotation(Quaternion newRotation){ rotation = newRotation; }
void Transform::setScale(Vector3 newScale){ scale = newScale; }

//Public Functions
Component* Transform::Clone(){ return new Transform(*this); }

void Transform::Update()
{
	Matrix4 rotationMatrix = rotation.getRotationMatrix();

	Matrix4 translationMatrix(1.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f, 0.0f,
						   0.0f, 0.0f, 1.0f, 0.0f,
						   position.getX(), position.getY(), position.getZ(), 1.0f);

	Matrix4 scaleMatrix(scale.getX(), 0.0f, 0.0f, 0.0f,
					 0.0f, scale.getY(), 0.0f, 0.0f,
					 0.0f, 0.0f, scale.getZ(), 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);

	modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;

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

		Camera* currentCamera = Camera::GetActiveCamera();
		Matrix4 viewMatrix = currentCamera->getViewMatrix();
		Matrix4 projectionMatrix = currentCamera->getProjectionMatrix();

		Matrix4 worldMatrix = (modelMatrix * viewMatrix) * projectionMatrix;

		material->setMatrix4("worldMatrix", worldMatrix);
		material->setMatrix4("modelMatrix", modelMatrix);
		material->setMatrix4("rotationMatrix", rotation.getRotationMatrix());
	}
}

Transform::~Transform()
{

}
