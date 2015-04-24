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

	Quaternion currentRotation = transform->getWorldRotation();

	Vector3 deltaEulerRot = rotationDelta * deltaTime;
	Quaternion deltaRotX = Quaternion(Vector4(1, 0, 0, deltaEulerRot[0]));
	Quaternion deltaRotY = Quaternion(Vector4(0, 1, 0, deltaEulerRot[1]));
	Quaternion deltaRotZ = Quaternion(Vector4(0, 0, 1, deltaEulerRot[2]));

	Quaternion deltaRot = deltaRotX * deltaRotY * deltaRotZ;

	Quaternion newRotation = deltaRot * currentRotation;

	transform->setWorldRotation(newRotation);
}


Spin::~Spin()
{
}
