#ifndef CAMERA_H
#define CAMERA_H

#define _USE_MATH_DEFINES

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
#include "Octree.h"

class Camera : public GameObject
{
public:
	static Octree* Camera::renderingOctree;

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
	virtual void OnRender() override;	

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

	GLint viewMatrixPos;
	GLint projectionMatrixPos;

	GLint lookAtPos;
	GLint eyePointPos;
	GLint upPos;

	Matrix4 calcViewMatrix();
	Matrix4 calcProjectionMatrix();

	void startGL(Material* material);
	void startD3D(Material* material);

	void prepRenderGL(Material* material);
	void prepRenderD3D(Material* material);

	void endRenderGL();
	void endRenderD3D();

};

#endif

