#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/GameInputManager.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;

//statics
RaycastHit GameInputManager::lastHit;

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
	Vector3 cameraPos = camera->getGameObject()->getTransform()->getPosition();

	Matrix4 inverseViewProj = camera->getViewMatrix() * camera->getProjectionMatrix();
	inverseViewProj = inverseViewProj.getInverse();

	Vector4 rayEyeNear = inverseViewProj * rayClipNear;
	Vector4 rayEyeFar = inverseViewProj * rayClipFar;
	
	Vector3 rayWorldNear = Vector3(rayEyeNear);
	Vector3 rayWorldFar = Vector3(rayEyeFar);

	Vector3 rayDirection = Vector3::Normalize(rayWorldFar - rayWorldNear);

	Ray ray(cameraPos, rayDirection);

	std::vector<RaycastHit> raycastHits;

	//Test ray against every collider we have in the scene
	std::unordered_map<Collider*, int> colliders = PhysicsManager::colliders;
	for (auto it = colliders.begin(); it != colliders.end(); it++)
	{
		Collider* collider = it->first;
		Vector3* pointOfCollision = new Vector3();

		//Any colliders we raycast against we need to store
		if (collider->isColliding(ray, pointOfCollision))
		{
			RaycastHit hit;
			hit.point = *pointOfCollision;
			hit.collider = collider;
			raycastHits.push_back(hit);
		}

		delete pointOfCollision;
	}

	float minDistance = std::numeric_limits<float>::max();
	RaycastHit closestHit;

	//See which point of collision is closest
	for (unsigned int i = 0; i < raycastHits.size(); i++)
	{
		RaycastHit hit = raycastHits[i];
		Vector3 difference = hit.point - cameraPos;
		float distance = difference.getMagnitude();

		if (distance < minDistance)
		{
			minDistance = distance;
			closestHit = hit;
		}
	}


	if (closestHit.collider != nullptr)
	{
		if (closestHit.collider == lastHit.collider)
		{
			closestHit.collider->getGameObject()->OnMouseStay();
			lastHit = closestHit;
			std::cout << "Stay" << std::endl;
		}
		else
		{
			closestHit.collider->getGameObject()->OnMouseOver();
			lastHit = closestHit;
			std::cout << "Over" << std::endl;
		}
	}
	else if (lastHit.collider != nullptr)
	{
		lastHit.collider->getGameObject()->OnMouseExit();
		lastHit = RaycastHit();
		std::cout << "End" << std::endl;
	}
	
}