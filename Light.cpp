#include "light.h"

Light::Light(GLint program)
{
	loadShader(program);

	position[0] = 0;
	position[1] = 0;
	position[2] = 0;
}

Light::Light(GLint program, float x, float y, float z)
{
	loadShader(program);
	
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

Light::Light(GLint program, Vector3 pos)
{
	loadShader(program);
	
	position[0] = pos.getX();
	position[1] = pos.getY();
	position[2] = pos.getZ();
}

void Light::display()
{
	glUseProgram(shaderProgram);

	glUniform3fv(shaderProgram, 1, position);

	glUseProgram(0);
}

void Light::loadShader(GLint program)
{
	position = new float[3];

	shaderProgram = program;

	glUseProgram(shaderProgram);

	lightPosLocation = glGetUniformLocation(shaderProgram, "lightPosition");

	glUseProgram(0);
}


Light::~Light(void)
{
}
