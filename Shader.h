#ifndef SHADER_H
#define SHADER_H

#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include "Component.h"

class Shader : public Component
{
public:
	Shader(char* vertexShaderFileName, char* fragmentShaderFileName);

	//Accessors
	GLuint getShaderProgram();

	GLuint getPositionLocation();
	GLuint getRotationLocation();
	GLuint getScaleLocation();

	//Component Overrides
	virtual void Start();
	virtual void Render();

	~Shader();

private:
	GLuint shaderProgram;

	GLuint positionLocation;
	GLuint rotationLocation;
	GLuint scaleLocation;

};

#endif
