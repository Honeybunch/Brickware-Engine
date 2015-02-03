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

#ifndef USE_D3D_ONLY
void Material::startGL()
{
	//Get positions of Uniforms
	GLint shaderProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	int totalUniforms = -1;
	glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &totalUniforms);
	
	//Build a map of all uniform locations in the bound shader
	for (int i = 0; i < totalUniforms; i++)
	{
		int nameLength = -1, num = -1;
		GLenum type = GL_ZERO;
		char* name = new char[100];

		glGetActiveUniform(shaderProgram, GLuint(i), sizeof(name) - 1,
			&nameLength, &num, &type, name);

		uniformMap[name] = i;
	}
}


#endif

#ifdef D3D_SUPPORT
void Material::startD3D()
{
	vsConstantBuffer = shader->getConstantBuffer();
}

#endif