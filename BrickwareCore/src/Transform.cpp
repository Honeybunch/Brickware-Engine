#define BRICKWARE_CORE_EXPORTS

#include "Transform.h"
#include "GameObject.h"

#include <iostream>

Transform::Transform()
{
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotation = Quaternion::getQuaternionIdentity();
	scale = Vector3(1.0f, 1.0f, 1.0f);

	Quaternion q1(Vector3(3.14/2.0f, 3.14/2.0f, 0));

	int i = 0;
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
	/*
	Vector3 eulerRotation = rotation.getEulerAngles();

	//Calculate Model Matrix
	Matrix4 rotationX(1.0f, 0.0f, 0.0f, 0.0f,
					  0.0f, cosf(eulerRotation.getX()), sinf(eulerRotation.getX()), 0.0f,
					  0.0f, -sinf(eulerRotation.getX()), cosf(eulerRotation.getX()), 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 rotationY(cosf(eulerRotation.getY()), 0.0f, -sinf(eulerRotation.getY()), 0.0f,
					  0.0f, 1.0f, 0.0f, 0.0f, 
					  sinf(eulerRotation.getY()), 0.0f, cosf(eulerRotation.getY()), 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 rotationZ(cosf(eulerRotation.getZ()), -sinf(eulerRotation.getZ()), 0.0f, 0.0f,
					  sinf(eulerRotation.getZ()), cosf(eulerRotation.getZ()), 0.0f, 0.0f,
				      0.0f, 0.0f, 1.0f, 0.0f, 
					  0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4 rotationMat = rotationX * rotationY * rotationZ;
	*/

	Matrix4 rotationMat = rotation.getRotationMatrix();

	Matrix4 translationMat(1.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f, 0.0f,
						   0.0f, 0.0f, 1.0f, 0.0f,
						   position.getX(), position.getY(), position.getZ(), 1.0f);

	Matrix4 scaleMat(scale.getX(), 0.0f, 0.0f, 0.0f,
					 0.0f, scale.getY(), 0.0f, 0.0f,
					 0.0f, 0.0f, scale.getZ(), 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);

	modelMatrix = scaleMat * rotationMat * translationMat;

	//Recalculate forward, right and up

	forward = Vector3::Normalize((modelMatrix * Vector3(0, 0, -1) - position));
	right = Vector3::Normalize((modelMatrix * Vector3(1, 0, 0) - position));
	up = Vector3::Normalize((modelMatrix * Vector3(0, 1, 0) - position));
}

void Transform::Render()
{
	GameObject* go = this->getGameObject();
	Material* material = go->getComponent<Material>();

	material->setMatrix4("modelMatrix", modelMatrix);
}

//Send info to GLSL Shader
void Transform::renderGL(Material* material)
{
	
}

//Send info to HLSL Shader
void Transform::renderD3D(Material* material)
{
	//TODO
}

Transform::~Transform()
{

}
