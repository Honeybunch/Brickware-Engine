#ifndef LIGHT_H
#define LIGHT_H

#include "Vector3.h"

#ifdef __APPLE__ 
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif

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

