#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/FirstPersonController.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;

FirstPersonController::FirstPersonController()
{
	speed = 1.0f;
}

void FirstPersonController::moveForward()
{
	Vector3 pos = getGameObject()->getTransform()->getPosition();
	Vector3 rot = getGameObject()->getTransform()->getEulerRotation();

	float deltaSpeed = speed * GameTime::GetDeltaTime();

	if (Input::getKeyDown(KeyCode::shift))
		deltaSpeed *= 5;

	pos.setX(pos.getX() - deltaSpeed * sin(rot.getY()));
	pos.setY(pos.getY() + deltaSpeed * sin(rot.getX()));
	pos.setZ(pos.getZ() - deltaSpeed * cos(rot.getY()));

	getGameObject()->getTransform()->setPosition(pos);
}

void FirstPersonController::moveBackward()
{
	Vector3 pos = getGameObject()->getTransform()->getPosition();
	Vector3 rot = getGameObject()->getTransform()->getEulerRotation();

	float deltaSpeed = speed * GameTime::GetDeltaTime();

	if (Input::getKeyDown(KeyCode::shift))
		deltaSpeed *= 5;

	pos.setX(pos.getX() + deltaSpeed * sin(rot.getY()));
	pos.setZ(pos.getZ() + deltaSpeed * cos(rot.getY()));

	getGameObject()->getTransform()->setPosition(pos);
}

void FirstPersonController::moveLeft()
{
	Vector3 pos = getGameObject()->getTransform()->getPosition();
	Vector3 rot = getGameObject()->getTransform()->getEulerRotation();

	float deltaSpeed = speed * GameTime::GetDeltaTime();

	if (Input::getKeyDown(KeyCode::shift))
		deltaSpeed *= 5;

	pos.setX(pos.getX() - deltaSpeed * cos(rot.getY()));
	pos.setZ(pos.getZ() + deltaSpeed * sin(rot.getY()));

	getGameObject()->getTransform()->setPosition(pos);
}

void FirstPersonController::moveRight()
{
	Vector3 pos = getGameObject()->getTransform()->getPosition();
	Vector3 rot = getGameObject()->getTransform()->getEulerRotation();

	float deltaSpeed = speed * GameTime::GetDeltaTime();

	if (Input::getKeyDown(KeyCode::shift))
		deltaSpeed *= 5;

	pos.setX(pos.getX() + deltaSpeed * cos(rot.getY()));
	pos.setZ(pos.getZ() - deltaSpeed * sin(rot.getY()));

	getGameObject()->getTransform()->setPosition(pos);
}

void FirstPersonController::Update()
{
	//Handle Input
	if (Input::getKeyDown(KeyCode::w))
		moveForward();

	if (Input::getKeyDown(KeyCode::a))
		moveLeft();

	if (Input::getKeyDown(KeyCode::s))
		moveBackward();

	if (Input::getKeyDown(KeyCode::d))
		moveRight();

	float screenCenterX = Screen::getWidth() / 2.0f;
	float screenCenterY = Screen::getHeight() / 2.0f;

	//Adjust yaw and pitch based on how much we've deviated from the center
	float yawDiff = (screenCenterX - Input::getMousePosition().getX()) / Screen::getWidth();
	float pitchDiff = (screenCenterY - Input::getMousePosition().getY()) / Screen::getHeight();

	Transform* transform = getGameObject()->getTransform();
	Vector3 rotation = transform->getEulerRotation();

	rotation.setX(rotation.getX() + pitchDiff);
	rotation.setY(rotation.getY() + yawDiff);

	transform->setEulerRotation(rotation);

	Input::setMousePosition(Vector2(screenCenterX, screenCenterY));
}
