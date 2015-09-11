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
}

void Light::setAmbientColor(Vector3 ambientColor){ this->ambientColor = ambientColor; }
void Light::setDiffuseColor(Vector3 diffuseColor){ this->diffuseColor = diffuseColor; }
void Light::setSpecularColor(Vector3 specularColor){ this->specularColor = specularColor; }

Light::~Light(void)
{
}
