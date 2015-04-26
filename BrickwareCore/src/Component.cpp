#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore\Component.hpp"
#include "BrickwareCore\Collider.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;

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
void Component::LateUpdate(){}
void Component::FixedUpdate(){}

void Component::OnCollision(Collision* other){}
void Component::OnTrigger(Collision* other){}

void Component::OnMouseOver(){}
void Component::OnMouseStay(){}
void Component::OnMouseExit(){}

void Component::Render(){}

#ifdef _DEBUG
void Component::DebugDraw(){}
#endif

Component::~Component()
{
}
