#include "Material.h"

Material::Material(Shader* shader)
{
	shaderProgram = shader->getGLShaderProgram();
}

//Accessors
GLuint Material::getShaderProgram(){return shaderProgram;}

GLuint Material::getModelMatrixPos(){return modelMatrixPos;}

//Component Overrides
void Material::Start()
{
	//Get positions of Uniforms
	modelMatrixPos = glGetUniformLocation(shaderProgram, "modelMatrix");
}