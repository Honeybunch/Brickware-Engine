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

	Vector4 rayClip(x, y, -1.0f, 1.0f);

	//Get the ray from clip space to eye space
	Camera* camera = Camera::GetActiveCamera();

	Vector4 rayEye;
	Matrix4 projection = camera->getProjectionMatrix();

	rayEye = projection.getInverse() * rayClip;
	rayEye = Vector4(rayEye[0], rayEye[1], -1.0f, 0.0f);

	//Then from eye space to world space
	Vector3 rayWorld;
	Matrix4 view = camera->getViewMatrix();

	Vector4 homogenousWorld = view.getInverse() * rayEye;
	rayWorld = Vector3::Normalize(Vector3(homogenousWorld[0], homogenousWorld[1], homogenousWorld[2]));

	//Actually create the day object based off of the camera's position
	Vector3 cameraPos = camera->getGameObject()->getTransform()->getPosition();

	Ray ray(cameraPos, camera->getGameObject()->getTransform()->getForward());

	Graphics::Primitive::DrawLine(cameraPos, cameraPos + rayWorld);

	//Test ray against every collider we have in the scene
	std::unordered_map<Collider*, int> colliders = PhysicsManager::colliders;
	for (auto it = colliders.begin(); it != colliders.end(); it++)
	{
		Collider* collider = it->first;
		
		if (collider->isColliding(ray))
		{
			collider->getGameObject()->OnMouseOver();
		}
	}
}