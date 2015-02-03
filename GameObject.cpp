#include "GameObject.h"
#include "Octree.h"
#include "Camera.h"
#include "Settings.h"
#include "Game.h"

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

vector<GameObject*> GameObject::getGameObjects(){ return Game::gameObjects; }

//Accessors
Transform* GameObject::getTransform(){ return transform; }

vector<Component*>& GameObject::getComponents(){ return components;  }

//Mutators
void GameObject::addComponent(Component* newComponent)
{
	newComponent->setGameObject(this);
	components.push_back(newComponent);

	componentCount = components.size();
}

void GameObject::Start()
{
	Material* material = getComponent<Material>();

	if (material)
		material->bindShader();

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Start();

	if (material)
		material->freeShader();
}

void GameObject::Update()
{
	Material* material = getComponent<Material>();

	if (material)
		material->bindShader();

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Update();

	if (material)
		material->freeShader();
}

void GameObject::OnRender()
{
	Material* material = getComponent<Material>();

	if (material)
		material->bindShader();

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Render();

	if (material)
		material->freeShader();
}

GameObject::~GameObject()
{
	for (unsigned int i = 0; i < components.size(); i++)
		delete components[i];

	for (unsigned int i = 0; i < Game::gameObjects.size(); i++)
	{
		if (Game::gameObjects[i] == this)
			Game::gameObjects.erase(Game::gameObjects.begin() + i, Game::gameObjects.begin() + 1 + i);
	}
}
