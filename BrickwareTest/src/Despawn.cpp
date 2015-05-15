#include "Despawn.hpp"

using namespace Brickware;
using namespace Core;

Despawn::Despawn(){}

void Despawn::Update()
{
	if (getGameObject()->getTransform()->getPosition()[1] < -10.0f)
		GameObject::Destroy(getGameObject());
}

Despawn::~Despawn(){}