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

void Component::Start(){}

void Component::Update(){}

void Component::Render(){}

Component::~Component()
{
}
