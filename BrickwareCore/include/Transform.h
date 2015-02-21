#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "BrickwareCoreDLL.h"

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

class GameObject;

#include "Shader.h"
#include "Component.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Quaternion.h"

class BRICKWARE_CORE_API Transform : public Component
{
public:
	Transform();

	//Accessors
	Vector3 getPosition();
	Vector3 getEulerRotation();
	Quaternion getRotation();
	Vector3 getScale();

	Vector3 getForward();
	Vector3 getRight();
	Vector3 getUp();

	Matrix4 getModelMatrix();
	
	//Mutators
	void setPosition(Vector3 newPosition);
	void setEulerRotation(Vector3 newRotation);
	void setRotation(Quaternion newRotation);
	void setScale(Vector3 newScale);

	//Component Overrides
	virtual Component* Clone() override;
	virtual void Update();
	virtual void Render();

	~Transform();

private:
	Vector3 position;
	Quaternion rotation;
	Vector3 eulerRotation;
	Vector3 scale;

	Vector3 forward;
	Vector3 right;
	Vector3 up;

	Matrix4 modelMatrix;

	void renderGL(Material* material);
	void renderD3D(Material* material);
};

#endif