#include "light.h"

Light::Light()
{
	
}

void Light::Start()
{
	Material* material = getGameObject()->getComponent<Material>();

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		startD3D(material);
	else
		startGL(material);
#elif defined(USE_D3D_ONLY)
	startD3D(material);
#else
	startGL(material);
#endif
}

void Light::Render()
{
	Material* material = getGameObject()->getComponent<Material>();

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		renderD3D(material);
	else
		renderGL(material);
#elif defined(USE_D3D_ONLY)
	renderD3D(material);
#else
	renderGL(material);
#endif
}

void Light::startGL(Material* material)
{
	GLint shaderProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	lightPosLocation = glGetUniformLocation(shaderProgram, "lightPosition");
}
void Light::startD3D(Material* material)
{
	//TODO
}

void Light::renderGL(Material* material)
{
	glUniform3fv(lightPosLocation, 1, getGameObject()->getTransform()->getPosition()->getAsArray());
}
void Light::renderD3D(Material* material)
{
	//TODO
}


Light::~Light(void)
{
}
