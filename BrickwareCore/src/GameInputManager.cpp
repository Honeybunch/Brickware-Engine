#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore\GameInputManager.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;

void GameInputManager::Initialize()
{
	
}

void GameInputManager::Update()
{
	MouseOverObjects();
}

void GameInputManager::Destroy()
{
	
}

void GameInputManager::MouseOverObjects()
{
	Vector2 mousePos = Input::getMousePosition();

	//Resolution to screen space
	float x = ((2.0f * mousePos[0]) / Screen::getWidth()) - 1.0f;
	float y = 1.0f - (2.0f * mousePos[1]) / Screen::getHeight();

	Vector4 rayClipNear(x, y, -1.0f, 1.0f);
	Vector4 rayClipFar(x, y, 1.0f, 1.0f);

	//Get the ray from clip space to eye space
	Camera* camera = Camera::GetActiveCamera();

	Matrix4 inverseViewProj = camera->getProjectionMatrix() * camera->getViewMatrix();
	inverseViewProj = inverseViewProj.getInverse().getTranspose();

	Vector4 rayEyeNear = inverseViewProj * rayClipNear;
	Vector4 rayEyeFar = inverseViewProj * rayClipFar;
	
	Vector3 rayWorldNear = Vector3(rayEyeNear);
	Vector3 rayWorldFar = Vector3(rayEyeFar);

	Vector3 rayDirection = Vector3::Normalize(rayWorldFar - rayWorldNear);

	Ray ray(camera->getGameObject()->getTransform()->getPosition(), rayDirection);

	//Test ray against every collider we have in the scene
	std::unordered_map<Collider*, int> colliders = PhysicsManager::colliders;
	for (auto it = colliders.begin(); it != colliders.end(); it++)
	{
		Collider* collider = it->first;
		
		if (collider->isColliding(ray))
		{
			std::cout << "Colliding" << std::endl;
			collider->getGameObject()->OnMouseOver();
		}
	}
}