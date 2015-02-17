#include "Spawner.h"


Spawner::Spawner(GameObject* gameObject)
{
	this->spawnable = *gameObject;
}

void Spawner::Update()
{
	if (Input::getKeyDown(KeyCode::space))
	{
		spawnObject();
	}
}

void Spawner::spawnObject()
{
	GameObject* newObject = new GameObject(spawnable);

	Camera* camera = getGameObject()->getComponent<Camera>();
	Vector3 spherePos = Vector3(camera->getLookAt());

	newObject->getTransform()->setPosition(spherePos);
	newObject->getTransform()->setScale(Vector3(0.1f, 0.1f, 0.1f));
	newObject->getTransform()->setRotation(Vector3(0.0f, 0.0f, 0.0f));

	newObject->Start();

	//Check if the spheres are colliding
	Collider* collider = newObject->getComponent<Collider>();

	bool collided = false;

	if (collider)
	{
		for (unsigned int i = 0; i < GameObject::getGameObjects().size(); i++)
		{
			GameObject* otherObject = GameObject::getGameObjects()[i];
			Collider* otherCollider = otherObject->getComponent<Collider>();

			//If the game object we're looking at has a collider
			if (otherCollider)
			{
				if (otherCollider->isColliding(collider) && otherObject != newObject)
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
