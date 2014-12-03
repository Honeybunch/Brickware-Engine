#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES 1

#include <iostream>
#include <vector>
#include <math.h>

#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>


#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera(float FoV, float width, float height, float zNear, float zFar);
	Camera(Transform* transform, float FoV, float width, float height, float zNear, float zFar);
	~Camera(void);

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();

	virtual void Start() override;
	virtual void Update() override;
	virtual void OnRender() override;	

	GLfloat yaw;
	GLfloat pitch;

private:
	GLfloat FoV;
	GLfloat width;
	GLfloat height;
	GLfloat zNear;
	GLfloat zFar;

	GLfloat speed;

	Vector3* lookAt;
	Vector3* up;

	GLint FoVPos;
	GLint widthPos;
	GLint heightPos;
	GLint zNearPos;
	GLint zFarPos;

	GLint yawPos;
	GLint pitchPos;

	GLint lookAtPos;
	GLint eyePointPos;
	GLint upPos;
};

#endif

