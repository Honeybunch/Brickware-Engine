#include "Spin.h"

Spin::Spin()
{
	rotationDelta = Vector3();
}

void Spin::Start()
{
	transform = getGameObject()->getTransform();
}

void Spin::Update()
{
	Vector3 currentRotation = transform->getRotation();
	Vector3 newRotation = currentRotation + rotationDelta;

	transform->setRotation(newRotation);
}


Spin::~Spin()
{
}
