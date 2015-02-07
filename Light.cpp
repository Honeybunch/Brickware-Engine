#include "light.h"

Light::Light()
{
	
}

void Light::Start()
{
}

void Light::Render()
{
	Material* material = getGameObject()->getComponent<Material>();

	material->setVector3("lightPosition", getGameObject()->getTransform()->getPosition());
}

Light::~Light(void)
{
}
