#ifndef LIGHT_H
#define LIGHT_H

#include "Vector3.h"

#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>

class Light
{
public:
	Light(GLint program);
	Light(GLint program, Vector3 pos);
	Light(GLint program, float x, float y, float z);

	virtual void display();

	~Light(void);

private:
	GLint shaderProgram;

	GLuint lightPosLocation;

	float* position;

	void loadShader(GLint program);

};

#endif

