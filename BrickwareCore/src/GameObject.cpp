#define BRICKWARE_CORE_EXPORTS

#include "Octree.h"
#include "Camera.h"
#include "Settings.h"
#include "Game.h"
#include "GameObject.h"

using namespace Brickware;
using namespace Core;
using namespace Math;
using namespace Graphics;

GameObject::GameObject()
{
	transform = new Transform();

	addComponent(transform);

	Game::gameObjects.push_back(this);
}

GameObject::GameObject(Transform* transform)
{
	GameObject::transform = transform;

	addComponent(GameObject::transform);

	Game::gameObjects.push_back(this);
}

GameObject::GameObject(GameObject& other)
{
	components = vector<Component*>();

	//Copy components that are able to be copied over
	unsigned int componentCount = other.components.size();

	for (unsigned int i = 0; i < componentCount; i++)
	{
		Component* copiedComponent = other.components[i]->Clone();
		if (copiedComponent)
		{
			components.push_back(copiedComponent);
			copiedComponent->setGameObject(this);
		}
	}

	//Determine which of the copied components is the Transform
	for (unsigned int i = 0; i < componentCount; i++)
	{
		Transform* newTransform = dynamic_cast<Transform*>(components[i]);
		if (newTransform)
		{
			transform = newTransform;
			break;
		}
	}

	Game::gameObjects.push_back(this);
}

vector<GameObject*> GameObject::getGameObjects(){ return Game::gameObjects; }

//Accessors
Transform* GameObject::getTransform(){ return transform; }

vector<Component*>& GameObject::getComponents(){ return components;  }

//Mutators
void GameObject::addComponent(Component* newComponent)
{
	newComponent->setGameObject(this);
	components.push_back(newComponent);
}

void GameObject::Start()
{
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Start();
}

void GameObject::Update()
{
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Update();
}

void GameObject::FixedUpdate()
{
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->FixedUpdate();
}

void GameObject::OnCollisionEnter(Collision* collision)
{
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->OnCollision(collision);
}

void GameObject::OnRender()
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		components[i]->Render();
#ifdef _DEBUG
		if (Debug::Debugging)
			components[i]->DebugDraw();
#endif
	}
}

GameObject::~GameObject()
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		delete components[i];
		components[i] = NULL;
	}

	//Before the GameObject is deleted make sure to remove it from the collection
	for (unsigned int i = 0; i < Game::gameObjects.size(); i++)
	{
		if (Game::gameObjects[i] == this)
			Game::gameObjects.erase(Game::gameObjects.begin() + i, Game::gameObjects.begin() + 1 + i);
	}
}
