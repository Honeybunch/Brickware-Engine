#ifndef CAMERA_H
#define CAMERA_H

#include "BrickwareCoreDLL.h"

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

#include "Transform.h"
#include "Matrix4.h"
#include "GameObject.h"

//Used to avoid warnings about exporting std::vectors
class Camera;
template class BRICKWARE_CORE_API std::vector <Camera*>;

class BRICKWARE_CORE_API Camera : public Component
{
public:
	static Camera* GetActiveCamera();

	Camera(float FoV, float width, float height, float zNear, float zFar);
	Camera(Transform* transform, float FoV, float width, float height, float zNear, float zFar);
	~Camera(void);

	Vector3 getLookAt();
	Matrix4 getViewMatrix();
	Matrix4 getProjectionMatrix();
	
	void setLookAt(Vector3 lookAt);

	void setActive();

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();

	virtual void Start() override;
	virtual void Update() override;
	void Render(Material* material);	

private:
	static std::vector<Camera*> SceneCameras;
	static Camera* ActiveCamera;

	bool active = false;

	float FoV;
	float width;
	float height;
	float zNear;
	float zFar;

	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;

	float speed;
	Vector3 lookAt;

	Matrix4 calcViewMatrix();
	Matrix4 calcProjectionMatrix();
};

#endif

