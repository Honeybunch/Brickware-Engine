#include "Spin.hpp"

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;
using namespace Utility;

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
	float deltaTime = GameTime::GetDeltaTime();

	Vector3 currentRotation = transform->getEulerRotation();
	Vector3 newRotation = (currentRotation + (rotationDelta * deltaTime));

	transform->setEulerRotation(newRotation);
}


Spin::~Spin()
{
}
