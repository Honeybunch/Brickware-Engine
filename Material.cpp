#include "Material.h"

Material::Material(Shader* shader)
{
	this->shader = shader;
}

void Material::bindShader()
{
	shader->bindShader();
}

void Material::freeShader()
{
	shader->freeShader();
}

//Accessors

GLuint Material::getModelMatrixPos(){return modelMatrixPos;}

//Component Overrides
void Material::Start()
{	
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		startD3D();
	else
		startGL();
#elif defined(USE_D3D_ONLY)
	startD3D();
#else
	startGL();
#endif
}

void Material::startGL()
{
	//Get positions of Uniforms
	GLint shaderProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	modelMatrixPos = glGetUniformLocation(shaderProgram, "modelMatrix");
}

void Material::startD3D()
{
	//TODO
}