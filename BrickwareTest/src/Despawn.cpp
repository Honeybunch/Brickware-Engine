#include "Despawn.h"

using namespace Brickware;
using namespace Core;

Despawn::Despawn(){}

void Despawn::Update()
{
	if (getGameObject()->getTransform()->getPosition()[1] < -10.0f)
		delete getGameObject();
}

Despawn::~Despawn(){}