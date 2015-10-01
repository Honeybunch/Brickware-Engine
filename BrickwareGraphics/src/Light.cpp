#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Light.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

Light::Light()
{
	//Default light colors to perfectly white
	ambientColor = Vector3(.1f, .1f, .1f);
	diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	specularColor = Vector3(1.0f, 1.0f, 1.0f);

	shadowMapRes = GraphicsSettings::ShadowQual;
}

void Light::setAmbientColor(Vector3 ambientColor){ this->ambientColor = ambientColor; }
void Light::setDiffuseColor(Vector3 diffuseColor){ this->diffuseColor = diffuseColor; }
void Light::setSpecularColor(Vector3 specularColor){ this->specularColor = specularColor; }

void Light::setShadowStrength(float shadowStrength){ this->shadowStrength = shadowStrength; }
void Light::setShadowBias(float shadowBias){ this->shadowBias = shadowBias; }

void Light::setShadowMapQuality(ShadowQuality shadowQuality)
{
	if (shadowQuality == ShadowQuality::UseQualSetting)
		shadowMapRes = GraphicsSettings::ShadowQual;
	else
		shadowMapRes = shadowQuality;

	//Reinit light so that the shadow maps are the right size
	Init();
}

Light::~Light(void)
{
}
