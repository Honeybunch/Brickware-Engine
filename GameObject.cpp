#include "GameObject.h"
#include "Octree.h"
#include "Camera.h"
#include "Settings.h"

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

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		prepRenderD3D(material);
	else
		prepRenderGL(material);
#elif defined(USE_D3D_ONLY)
	prepareRenderD3D(material);
#else
	prepareRenderGL(material);
#endif

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Render();

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		endRenderD3D();
	else
		endRenderGL();
#elif defined(USE_D3D_ONLY)
	endD3D();
#else
	endGL();
#endif
}

void GameObject::prepRenderGL(Material* material)
{
	GLuint shaderProgram = material->getShaderProgram();
	glUseProgram(shaderProgram);
}
void GameObject::prepRenderD3D(Material* material)
{
	//TODO
}

void GameObject::endRenderGL(){ glUseProgram(0); }
void GameObject::endRenderD3D(){ /*TODO*/ }

GameObject::~GameObject()
{
	for (unsigned int i = 0; i < components.size(); i++)
		delete components[i];
}
