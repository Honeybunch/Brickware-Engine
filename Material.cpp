#include "Material.h"

Material::Material(Shader* shader)
{
	shaderProgram = shader->getShaderProgram();
}

//Accessors
GLuint Material::getShaderProgram(){return shaderProgram;}

GLuint Material::getPositionLocation(){return positionLocation;}
GLuint Material::getRotationLocation(){return rotationLocation;}
GLuint Material::getScaleLocation(){return scaleLocation;}

//Component Overrides
void Material::Start()
{
	//Get positions of Uniforms
	positionLocation = glGetUniformLocation(shaderProgram, "trans");
	rotationLocation = glGetUniformLocation(shaderProgram, "theta");
	scaleLocation = glGetUniformLocation(shaderProgram, "scale");
}