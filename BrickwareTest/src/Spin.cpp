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
	float deltaTime = GameTime::getDeltaTime();

	Vector3 currentRotation = transform->getRotation();
	Vector3 newRotation = (currentRotation + (rotationDelta * deltaTime));

	transform->setRotation(newRotation);
}


Spin::~Spin()
{
}
