#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/InternalLight.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

InternalLight::InternalLight()
{
	//Default light colors to perfectly white
	ambientColor = Vector3(.1f, .1f, .1f);
	diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	specularColor = Vector3(1.0f, 1.0f, 1.0f);

	shadowStrength = 0.5f;
	shadowBias = 0.005f;

	shadowMapRes = GraphicsSettings::ShadowQual;
}

void InternalLight::setAmbientColor(Vector3 ambientColor){ this->ambientColor = ambientColor; }
void InternalLight::setDiffuseColor(Vector3 diffuseColor){ this->diffuseColor = diffuseColor; }
void InternalLight::setSpecularColor(Vector3 specularColor){ this->specularColor = specularColor; }

void InternalLight::setShadowStrength(float shadowStrength){ this->shadowStrength = shadowStrength; }
void InternalLight::setShadowBias(float shadowBias){ this->shadowBias = shadowBias; }

void InternalLight::setShadowMapQuality(ShadowQuality shadowQuality)
{
	if (shadowQuality == ShadowQuality::UseQualSetting)
		shadowMapRes = GraphicsSettings::ShadowQual;
	else
		shadowMapRes = shadowQuality;

	//Reinit light so that the shadow maps are the right size
	Init();
}

InternalLight::~InternalLight(void)
{
}
