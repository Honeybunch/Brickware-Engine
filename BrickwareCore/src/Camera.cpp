#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/FrustumCollider.hpp"
#include "BrickwareCore/Camera.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;

Camera* Camera::GetActiveCamera(){ return ActiveCamera; }

Camera::Camera(float FoV = 50, float width = 0.1f, float height = 0.1f, float zNear = 0.1f, float zFar = 100.0f)
{
	this->FoV = FoV;
	this->width = width;
	this->height = height;
	this->zNear = zNear;
	this->zFar = zFar;

	lookAt = Vector3();

	SceneCameras.push_back(this);
}

void Camera::Start()
{
	//getGameObject()->addComponent(new FrustumCollider(zNear, zFar, FoV, width / height));
}

Vector3 Camera::getLookAt(){ return lookAt; }
Matrix4 Camera::getViewMatrix(){ return viewMatrix; }
Matrix4 Camera::getProjectionMatrix(){ return projectionMatrix; }

void Camera::setLookAt(Vector3 lookAt){ this->lookAt = lookAt; }

void Camera::setActive()
{
	for (unsigned int i = 0; i < SceneCameras.size(); i++)
	{
		if (SceneCameras[i]->active == true)
			SceneCameras[i]->active = false;
	}

	ActiveCamera = this;
	active = true;
}

void Camera::Update()
{
	//Recalculate lookAt
	Vector3 position = getGameObject()->getTransform()->getPosition();
	Vector3 forward = getGameObject()->getTransform()->getForward();

	Vector3 newLookAt = position + forward;
	
	lookAt = newLookAt;

	viewMatrix = calcViewMatrix();
	projectionMatrix = calcProjectionMatrix();
}

void Camera::Render()
{	
	for (unsigned int i = 0; i < Graphics::Material::Materials.size(); i++)
	{
		Graphics::Material* material = Graphics::Material::Materials[i];

		material->setVector3("lookAt", lookAt);
		material->setVector3("eyePoint", getGameObject()->getTransform()->getPosition());
		material->setVector3("up", getGameObject()->getTransform()->getUp());
	}
}

/* 
	Private Methods and Statics
*/

std::vector<Camera*> Camera::SceneCameras;
Camera* Camera::ActiveCamera;

Matrix4 Camera::calcViewMatrix()
{
	Transform* transform = getGameObject()->getTransform();

	Vector3 eye = transform->getPosition();
	Vector3 up = transform->getUp();

	return Matrix4::getLookAtView(eye, lookAt, up);
}

Matrix4 Camera::calcProjectionMatrix()
{
	return Matrix4::getPerspectiveProjection(FoV, width, height, zNear, zFar);
}

Camera::~Camera(void)
{
}
