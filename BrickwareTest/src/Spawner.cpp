#include "Spawner.h"
#include <iostream>

Spawner::Spawner(GameObject* gameObject)
{
	this->spawnable = gameObject;
}

void Spawner::Update()
{
	if (Input::getKeyDown(KeyCode::space))
	{
		spawnObject();
		keyDown = true;
	}
}

void Spawner::spawnObject()
{
	GameObject* newObject = new GameObject(*spawnable);

	Camera* camera = getGameObject()->getComponent<Camera>();
	Vector3 spherePos = Vector3(camera->getLookAt());

	newObject->getTransform()->setPosition(spherePos);
	newObject->getTransform()->setScale(Vector3(0.1f, 0.1f, 0.1f));

	float xRot = (float)((rand() % 314) / 100) + 1;
	float yRot = (float)((rand() % 314) / 100) + 1;
	float zRot = (float)((rand() % 314) / 100) + 1;

	newObject->getTransform()->setRotation(Vector3(xRot, yRot, zRot));

	newObject->Start();

	//Check if the spheres are colliding
	Collider* collider = newObject->getComponent<Collider>();

	bool collided = false;

	if (collider)
	{
		unsigned int gameObjectCount = GameObject::getGameObjects().size();
		for (unsigned int i = 0; i < gameObjectCount; i++)
		{
			GameObject* otherObject = GameObject::getGameObjects()[i];
			Collider* otherCollider = otherObject->getComponent<Collider>();

			//If the game object we're looking at has a collider
			if (otherCollider)
			{
				if (otherObject != newObject && otherCollider->isColliding(collider))
				{
					collided = true;
					break;
				}
			}
		}
	}

	if (collided)
	{
		delete newObject;
	}
}

Spawner::~Spawner()
{
}
