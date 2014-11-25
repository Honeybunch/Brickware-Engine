#include "Camera.h"

Camera::Camera(GLuint program)
{
	shaderProgram = program;

	//Get the locations of the shader variables on the GPU
	FoVPos = glGetUniformLocation(shaderProgram, "FoV");
	widthPos = glGetUniformLocation(shaderProgram, "width");
	heightPos = glGetUniformLocation(shaderProgram, "height");
	zNearPos = glGetUniformLocation(shaderProgram, "zNear");
	zFarPos = glGetUniformLocation(shaderProgram, "zFar");

	lookAtPos = glGetUniformLocation(shaderProgram, "lookAt");
	eyePointPos = glGetUniformLocation(shaderProgram, "eyePoint");
	upPos = glGetUniformLocation(shaderProgram, "up");

	//Setup speed and velocity
	speed = 0.01f;

	//Setup look, eye, and up
	lookAt = new GLfloat[3];
	eyePoint = new GLfloat[3];
	up = new GLfloat[3];

	//Default yaw and pitch
	yaw = 0.0f;
	pitch = 0.0f;
}

void Camera::moveForward()
{
	eyePoint[0] -= speed * sin(yaw);
	eyePoint[2] -= speed * cos(yaw);

	updateCameraMath();
}

void Camera::moveBackward()
{
	eyePoint[0] += speed * sin(yaw);
	eyePoint[2] += speed * cos(yaw);

	updateCameraMath();
}

void Camera::moveLeft()
{
	eyePoint[0] -= speed * cos(yaw);
	eyePoint[2] += speed * sin(yaw);

	updateCameraMath();
}

void Camera::moveRight()
{
	eyePoint[0] += speed * cos(yaw);
	eyePoint[2] -= speed * sin(yaw);

	updateCameraMath();
}

void Camera::updateCameraMath()
{
	//Update the look at to account for position, pitch and yaw
	lookAt[0] = eyePoint[0] - 1*cos(pitch)*sin(yaw);
	lookAt[1] = eyePoint[1] + 1*sin(pitch);
	lookAt[2] = eyePoint[2] + -1*cos(pitch) * cos(yaw);

	up[0] = -1 * cos(pitch + M_PI/2.0) * sin(yaw);
	up[1] =  1 * sin(pitch + M_PI/2.0);
	up[2] = -1 * cos(pitch + M_PI/2.0) * cos(yaw);
}

void Camera::updateCamera()
{	
	updateCameraMath();

	glUseProgram(shaderProgram);

	glUniform1f(FoVPos, FoV);
	glUniform1f(zNearPos, zNear);
	glUniform1f(zFarPos, zFar);
	glUniform1f(widthPos, width);
	glUniform1f(heightPos, height);

	glUniform3fv (lookAtPos, 1, lookAt);
	glUniform3fv (eyePointPos, 1, eyePoint);
	glUniform3fv (upPos, 1, up);

	glUseProgram(0);
}

Camera::~Camera(void)
{
}
