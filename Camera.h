#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <math.h>

#include "Settings.h"

#ifdef D3D_SUPPORT
#include <windows.h>
#endif

#ifdef GL_SUPPORT
#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>
#endif

#include "GameObject.h"
#include "Octree.h"

class Camera : public Component
{
public:
	Octree* renderingOctree;

	Camera(float FoV, float width, float height, float zNear, float zFar);
	Camera(Transform* transform, float FoV, float width, float height, float zNear, float zFar);
	~Camera(void);

	Vector3* getLookAt();
	void setLookAt(Vector3* lookAt);

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;	

private:
	float FoV;
	float width;
	float height;
	float zNear;
	float zFar;

	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;

	float speed;
	Vector3* lookAt;

	Matrix4 calcViewMatrix();
	Matrix4 calcProjectionMatrix();

	void startGL(Material* material);
	void startD3D(Material* material);

	void renderGL(Material* material);
	void renderD3D(Material* material);
};

#endif

