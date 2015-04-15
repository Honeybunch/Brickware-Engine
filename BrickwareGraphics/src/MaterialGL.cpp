#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics\Material.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

#ifdef GL_SUPPORT

void Material::setVector4GL(const char* valueName, Vector4 value)
{
	GLuint uniformLocation = (GLuint)(shader->uniformMap[std::string(valueName)]);
	glUniform4fv(uniformLocation, 1, value.getAsArray());
}

void Material::setVector3GL(const char* valueName, Vector3 value)
{
	GLuint uniformLocation = (GLuint)(shader->uniformMap[std::string(valueName)]);
	glUniform3fv(uniformLocation, 1, value.getAsArray());
}

void Material::setVector2GL(const char* valueName, Vector2 value)
{
	GLuint uniformLocation = (GLuint)(shader->uniformMap[std::string(valueName)]);
	glUniform2fv(uniformLocation, 1, value.getAsArray());
}

void Material::setIntGL(const char* valueName, int value)
{
	GLuint uniformLocation = (GLuint)(shader->uniformMap[std::string(valueName)]);
	glUniform1i(uniformLocation, value);
}

void Material::setFloatGL(const char* valueName, float value)
{
	GLuint uniformLocation = (GLuint)(shader->uniformMap[std::string(valueName)]);
	glUniform1f(uniformLocation, value);
}

void Material::setDoubleGL(const char* valueName, double value)
{
	GLuint uniformLocation = (GLuint)(shader->uniformMap[std::string(valueName)]);
	glUniform1d(uniformLocation, value);
}

void Material::setMatrix4GL(const char* valueName, Matrix4 value)
{
	GLuint uniformLocation = (GLuint)(shader->uniformMap[std::string(valueName)]);
	glUniformMatrix4fv(uniformLocation, 1, false, value.getAsArray());
}

void Material::setMatrix3GL(const char* valueName, Matrix3 value)
{
	GLuint uniformLocation = (GLuint)(shader->uniformMap[std::string(valueName)]);
	glUniformMatrix3fv(uniformLocation, 1, false, value.getAsArray());
}

#endif