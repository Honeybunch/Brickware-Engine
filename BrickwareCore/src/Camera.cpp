#define BRICKWARE_CORE_EXPORTS

#include "FrustumCollider.h"
#include "Camera.h"

Camera* Camera::GetActiveCamera(){ return ActiveCamera; }

Camera::Camera(float FoV = 50, float width = 0.1f, float height = 0.1f, float zNear = 0.1f, float zFar = 100.0f)
{
	this->FoV = FoV;
	this->width = width;
	this->height = height;
	this->zNear = zNear;
	this->zFar = zFar;

	//Setup speed and velocity
	speed = 1.0f;

	lookAt = Vector3();

	SceneCameras.push_back(this);
}

void Camera::Start()
{
	//getGameObject()->addComponent(new FrustumCollider(zNear, zFar, FoV, width / height));
}

Vector3 Camera::getLookAt(){ return lookAt; }
Matrix4 Camera::getViewMatrix(){ return viewMatrix; }
Matrix4 Camera::getProjectionMatrix(){ return projectionMatrix; }

void Camera::setLookAt(Vector3 lookAt){ this->lookAt = lookAt; }

void Camera::setActive()
{
	for (unsigned int i = 0; i < SceneCameras.size(); i++)
	{
		if (SceneCameras[i]->active == true)
			SceneCameras[i]->active = false;
	}

	ActiveCamera = this;
	active = true;
}

void Camera::moveForward()
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

void Camera::moveBackward()
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

void Camera::moveLeft()
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

void Camera::moveRight()
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

void Camera::Update()
{
	//Recalculate lookAt
	Vector3 position = getGameObject()->getTransform()->getPosition();
	Vector3 forward = getGameObject()->getTransform()->getForward();

	Vector3 newLookAt = position + forward;
	
	lookAt = newLookAt;

	viewMatrix = calcViewMatrix();
	projectionMatrix = calcProjectionMatrix();

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

	Transform* cameraTransform = getGameObject()->getTransform();
	Vector3 cameraRot = cameraTransform->getEulerRotation();

	cameraRot.setX(cameraRot.getX() + pitchDiff);
	cameraRot.setY(cameraRot.getY() + yawDiff);

	cameraTransform->setEulerRotation(cameraRot);

	Input::setMousePosition(Vector2(screenCenterX, screenCenterY));
}

void Camera::Render(Material* material)
{	
	material->setVector3("lookAt", lookAt);
	material->setVector3("eyePoint", getGameObject()->getTransform()->getPosition());
	material->setVector3("up", getGameObject()->getTransform()->getUp());
}

/* 
	Private Methods and Statics
*/

std::vector<Camera*> Camera::SceneCameras;
Camera* Camera::ActiveCamera;

Matrix4 Camera::calcViewMatrix()
{
	Vector3 position = getGameObject()->getTransform()->getPosition();
	 
	//Calculate axes 
	Vector3 zAxis = Vector3::Normalize((position - lookAt));
	Vector3 xAxis = Vector3::Normalize(Vector3::Cross(getGameObject()->getTransform()->getUp(), zAxis));
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

	//Create view matrix;
	Matrix4 view(xAxis.getX(), yAxis.getX(), zAxis.getX(), 0, 
				 xAxis.getY(), yAxis.getY(), zAxis.getY(), 0,
				 xAxis.getZ(), yAxis.getZ(), zAxis.getZ(), 0,
				 Vector3::Dot(xAxis * -1, position),
				 Vector3::Dot(yAxis * -1, position),
				 Vector3::Dot(zAxis * -1, position), 
				 1);

	return view;
}

Matrix4 Camera::calcProjectionMatrix()
{
	float aspectRatio = width / height;

	float top = zNear * tanf(((float)M_PI / 180.0f) * (FoV * 2.0f));
	float bottom = -top;
	float right = top * aspectRatio;
	float left = -right;

	float depth = zFar - zNear;
	float q = -(zFar + zNear) / depth;
	float qn = -2 * (zFar * zNear) / depth;

	float w = 2 * zNear / width;
	w /= aspectRatio;
	float h = 2 * zNear / height;

	Matrix4 projection(w, 0, 0, 0,
					   0, h, 0, 0,
					   0, 0, q, -1,
					   0, 0, qn, 0);

	return projection;
}

Camera::~Camera(void)
{
}
