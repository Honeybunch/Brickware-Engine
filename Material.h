#ifndef MATERIAL_H
#define MATERIAL_H

#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

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