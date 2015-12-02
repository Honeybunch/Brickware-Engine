#include "Spawner.hpp"
#include <iostream>

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;
using namespace Utility;

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
	Vector3 spherePos = Vector3(camera->GetLookAt());
	Rigidbody* rigidbody = new Rigidbody();

	newObject->getTransform()->setPosition(spherePos);
	newObject->getTransform()->setScale(Vector3(0.2f, 0.2f, 0.2f));

	float xRot = (float)((rand() % 314) / 100.0f);
	float yRot = (float)((rand() % 314) / 100.0f);
	float zRot = (float)((rand() % 314) / 100.0f);

	//Calculate some random torque to apply
	float xTorque = (float)(rand() % 100) / 100.0f;
	float yTorque = (float)(rand() % 100) / 100.0f;
	float zTorque = (float)(rand() % 100) / 100.0f;

	newObject->getTransform()->setRotation(Vector3(xRot, yRot, zRot));
	newObject->addComponent(rigidbody);
	newObject->addComponent(new Despawn());

	newObject->Start();

	rigidbody->setMass(0.4f);
	rigidbody->addForce(Vector3::Normalize(camera->GetLookAt() - camera->getGameObject()->getTransform()->getPosition()) * 80.0f);
	rigidbody->addTorque(Vector3(xTorque, yTorque, zTorque));

	//Check if the spheres are colliding
 	Collider* collider = newObject->getComponent<Collider>();

	bool collided = false;

	if (collider)
	{
		size_t gameObjectCount = GameObject::getGameObjects().size();
		for (unsigned int i = 0; i < gameObjectCount; i++)
		{
			GameObject* otherObject = GameObject::getGameObjects()[i];
			Collider* otherCollider = otherObject->getComponent<Collider>();

			//If the game object we're looking at has a collider
			if (otherCollider)
			{
				if (otherObject != newObject && otherCollider->isColliding(collider, nullptr))
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
