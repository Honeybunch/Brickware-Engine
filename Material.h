#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>

#include "Shader.h"
#include "Component.h"

class Material : public Component
{
public:
	Material(Shader* shader);

	//Accessors
	GLuint getShaderProgram();

	GLuint getPositionLocation();
	GLuint getRotationLocation();
	GLuint getScaleLocation();

	//Component Overrides
	virtual void Start();

	~Material();

private:
	GLuint shaderProgram;

	GLuint positionLocation;
	GLuint rotationLocation;
	GLuint scaleLocation;

};

#endif