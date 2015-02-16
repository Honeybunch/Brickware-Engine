#define BRICKWARE_CORE_EXPORTS

#include "Component.h"

Component::Component()
{
}

//Accessors
GameObject* Component::getGameObject(){ return gameObject; }

//Mutators
void Component::setGameObject(GameObject* gameObject)
{
	Component::gameObject = gameObject;
}

//Virtual Functions

Component* Component::Clone(){ return NULL; }

void Component::Start(){}

void Component::Update(){}

void Component::Render(){}

Component::~Component()
{
}
