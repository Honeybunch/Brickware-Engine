#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/Light.hpp"
#include "BrickwareCore/GameObject.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;
using namespace Graphics;

Light::Light(LightType lightType)
{
	light = nullptr;
	initLight(lightType);
}

void Light::setDirection(Vector3 direction)
{
	((DirectionalLightInternal*)light)->setDirection(direction);
}

void Light::setDiffuseColor(Vector3 diffuseColor)
{ 
	light->setDiffuseColor(diffuseColor);
}
void Light::setSpecularColor(Vector3 specularColor)
{ 
	light->setSpecularColor(specularColor);
}

void Light::setShadowStrength(float shadowStrength)
{ 
	light->setShadowStrength(shadowStrength); 
}
void Light::setShadowBias(float shadowBias)
{ 
	light->setShadowBias(shadowBias); 
}

void Light::setShadowMapQuality(Graphics::ShadowQuality shadowQuality)
{ 
	light->setShadowMapQuality(shadowQuality); 
}

void Light::Render()
{
	(this->*RenderPtr)();
}

#ifdef BRICKWARE_DEBUG
void Light::DebugDraw()
{
	Graphics::Primitive::SetColor(Vector4(1, 1, 0, 1));
	Graphics::Primitive::SetPointSize(.3f);

	Graphics::Primitive::DrawPoint(getGameObject()->getTransform()->getPosition());
}
#endif

Light::~Light()
{
	delete light;
}

/*
	Private Methods
*/

void Light::initLight(LightType lightType)
{
	if (light != nullptr)
		delete light;

	switch (lightType)
	{
	case DIRECTIONAL:
		light = new DirectionalLightInternal();
		RenderPtr = &Light::RenderDirectional;
		break;
	case POINT:
		light = new PointLightInternal();
		RenderPtr = &Light::RenderPoint;
		break;
	}
}

void Light::RenderDirectional()
{
	RenderingManager::AddLight(light);
	light->RenderShadowMap();
}

void Light::RenderPoint()
{
	((PointLightInternal*)light)->setPosition(getGameObject()->getTransform()->getPosition());
	RenderingManager::AddLight(light);
	light->RenderShadowMap();
}