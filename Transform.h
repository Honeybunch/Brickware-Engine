#ifndef TRANSFORM_H
#define TRANSFORM_H

#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

class GameObject;

#include "Shader.h"
#include "Component.h"

#include "Vector3.h"
#include "Vector4.h"

class Transform : public Component
{
public:
	Transform();

	//Accessors
	Vector3* getPosition();
	Vector3* getRotation();
	Vector3* getScale();
	
	//Mutators
	void setPosition(Vector3* newPosition);
	void setRotation(Vector3* newRotation);
	void setScale(Vector3* newScale);

	//Component Overrides
	virtual void Render();

	~Transform();

private:
	Vector3* position;
	Vector3* rotation;
	Vector3* scale;


};

#endif