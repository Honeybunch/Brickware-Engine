#include "Material.h"

#ifndef USE_D3D_ONLY

void Material::startGL()
{
	//Get currently bound shader	
	GLint shaderProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	//Get positions of Uniforms
	int totalUniforms;
	glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &totalUniforms);

	//Build a map of all uniform locations
	for (int i = 0; i < totalUniforms; i++)
	{
		int nameLength, num;
		GLenum type = GL_ZERO;
		char* name = new char[100];

		glGetActiveUniform(shaderProgram, (GLuint)i, 99,
			&nameLength, &num, &type, name);

		name[nameLength] = 0;

		uniformMap[std::string(name)] = i;
	}
}

void Material::setVector4GL(char* valueName, Vector4 value)
{
	GLuint uniformLocation = (GLuint)(uniformMap[std::string(valueName)]);
	glUniform4fv(uniformLocation, 1, value.getAsArray());
}

void Material::setVector3GL(char* valueName, Vector3 value)
{
	GLuint uniformLocation = (GLuint)(uniformMap[std::string(valueName)]);
	glUniform3fv(uniformLocation, 1, value.getAsArray());
}

void Material::setVector2GL(char* valueName, Vector2 value)
{
	GLuint uniformLocation = (GLuint)(uniformMap[std::string(valueName)]);
	glUniform2fv(uniformLocation, 1, value.getAsArray());
}

void Material::setIntGL(char* valueName, int value)
{
	GLuint uniformLocation = (GLuint)(uniformMap[std::string(valueName)]);
	glUniform1i(uniformLocation, value);
}

void Material::setFloatGL(char* valueName, float value)
{
	GLuint uniformLocation = (GLuint)(uniformMap[std::string(valueName)]);
	glUniform1f(uniformLocation, value);
}

void Material::setDoubleGL(char* valueName, double value)
{
	GLuint uniformLocation = (GLuint)(uniformMap[std::string(valueName)]);
	glUniform1d(uniformLocation, value);
}

void Material::setMatrix4GL(char* valueName, Matrix4 value)
{
	GLuint uniformLocation = (GLuint)(uniformMap[std::string(valueName)]);
	glUniformMatrix4fv(uniformLocation, 1, false, value.getAsArray());
}

#endif