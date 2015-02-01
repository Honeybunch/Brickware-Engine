#include "Spawner.h"


Spawner::Spawner(Mesh* mesh, Shader* shader)
{
	this->mesh = mesh;
	this->shader = shader;
}

void Spawner::Update()
{
	if (Input::getKeyDown(KeyCode::space))
	{
		spawnSphere();
	}
}

void Spawner::spawnSphere()
{
	GameObject* newSphere = new GameObject();

	newSphere->addComponent(new Material(shader));
	newSphere->addComponent(new MeshRenderer(mesh));
	newSphere->addComponent(new SphereCollider(new Vector3(), 0.1f));

	Camera* camera = getGameObject()->getComponent<Camera>();
	Vector3* spherePos = new Vector3(*(camera->getLookAt()));

	newSphere->getTransform()->setPosition(spherePos);
	newSphere->getTransform()->setScale(new Vector3(0.1f, 0.1f, 0.1f));

	newSphere->Start();

	//Check if the spheres are colliding
	Collider* collider = newSphere->getComponent<Collider>();

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
				if (otherCollider->isColliding(collider) && otherObject != newSphere)
				{
					collided = true;
					break;
				}
			}
		}
	}

	if (collided)
	{
		delete newSphere;
	}
}

Spawner::~Spawner()
{
}
