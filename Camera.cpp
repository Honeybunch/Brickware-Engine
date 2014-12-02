#include "Camera.h"

Camera::Camera(float FoV = 50, float width = 0.1f, float height = 0.1f, float zNear = 0.1f, float zFar = 100.0f)
{
	this->FoV = FoV;
	this->width = width;
	this->height = height;
	this->zNear = zNear;
	this->zFar = zFar;

	//Setup speed and velocity
	speed = 0.1f;

	//Setup look, eye, and up
	lookAt = new Vector3(0, 0, 0);
	up = new Vector3(0, 1, 0);

	//Default yaw and pitch
	yaw = 0.0f;
	pitch = 0.0f;
}

void Camera::Start()
{
	Material* material = this->getComponent<Material>();
	GLuint shaderProgram = material->getShaderProgram();

	//Get the locations of the shader variables on the GPU
	FoVPos = glGetUniformLocation(shaderProgram, "FoV");
	widthPos = glGetUniformLocation(shaderProgram, "width");
	heightPos = glGetUniformLocation(shaderProgram, "height");
	zNearPos = glGetUniformLocation(shaderProgram, "zNear");
	zFarPos = glGetUniformLocation(shaderProgram, "zFar");

	lookAtPos = glGetUniformLocation(shaderProgram, "lookAt");
	eyePointPos = glGetUniformLocation(shaderProgram, "eyePoint");
	upPos = glGetUniformLocation(shaderProgram, "up");

	GameObject::Start();
}

void Camera::moveForward()
{
	Vector3* pos = this->transform->getPosition();

	pos->setX(pos->getX() - speed * sin(yaw));
	pos->setZ(pos->getZ() -  speed * cos(yaw));
}

void Camera::moveBackward()
{
	Vector3* pos = this->transform->getPosition();

	pos->setX(pos->getX() + speed * sin(yaw));
	pos->setZ(pos->getZ() + speed * cos(yaw));
}

void Camera::moveLeft()
{
	Vector3* pos = this->transform->getPosition();

	pos->setX(pos->getX() - speed * cos(yaw));
	pos->setZ(pos->getZ() + speed * sin(yaw));
}

void Camera::moveRight()
{
	Vector3* pos = this->transform->getPosition();

	pos->setX(pos->getX() + speed * cos(yaw));
	pos->setZ(pos->getZ() - speed * sin(yaw));
}

void Camera::Update()
{
	//Update the look at to account for position, pitch and yaw
	lookAt->setX(transform->getPosition()->getX() - 1*cos(pitch)*sin(yaw));
	lookAt->setY(transform->getPosition()->getY() + 1 * sin(pitch));
	lookAt->setZ(transform->getPosition()->getZ() + -1 * cos(pitch) * cos(yaw));

	up->setX( -1 * cos(pitch + M_PI/2.0) * sin(yaw));
	up->setY(1 * sin(pitch + M_PI / 2.0));
	up->setZ(-1 * cos(pitch + M_PI / 2.0) * cos(yaw));
}

void Camera::OnRender()
{	
	Material* material = this->getComponent<Material>();

	if (material == NULL)
	{
		cerr << "Error in Transform; no attached shader component" << endl;
		return;
	}

	GLuint shaderProgram = material->getShaderProgram();

	glUseProgram(shaderProgram);

	glUniform1f(FoVPos, FoV);
	glUniform1f(zNearPos, zNear);
	glUniform1f(zFarPos, zFar);
	glUniform1f(widthPos, width);
	glUniform1f(heightPos, height);

	glUniform3fv(lookAtPos, 1, lookAt->getAsArray());
	glUniform3fv(eyePointPos, 1, transform->getPosition()->getAsArray());
	glUniform3fv(upPos, 1, up->getAsArray());

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Render();

	glUseProgram(0);
}

Camera::~Camera(void)
{
}
