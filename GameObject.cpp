#include "GameObject.h"

GameObject::GameObject()
{
	transform = new Transform();

	addComponent(transform);
}

GameObject::GameObject(Transform* transform)
{
	GameObject::transform = transform;

	addComponent(GameObject::transform);
}

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
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Start();
}

void GameObject::Update()
{
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Update();
}

void GameObject::OnRender()
{
	Material* material = this->getComponent<Material>();

	if (material == NULL)
	{
		cerr << "Error in Transform; no attached shader component" << endl;
		return;
	}

	GLuint shaderProgram = material->getShaderProgram();

	glUseProgram(shaderProgram);

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Render();

	glUseProgram(0);
}

GameObject::~GameObject()
{
	//TODO: Should delete components
}
