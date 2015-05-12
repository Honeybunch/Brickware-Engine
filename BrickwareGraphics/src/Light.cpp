#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Light.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

int Light::LightCount;

Light::Light()
{
	//Default pos is 0,0,0
	position = Vector3();

	//Default light colors to perfectly white
	ambientColor = Vector3(.1f, .1f, .1f);
	diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	specularColor = Vector3(1.0f, 1.0f, 1.0f);

	lightIndex = LightCount;
	LightCount++;
}

void Light::setPosition(Vector3 position){ this->position = position; }
void Light::setAmbientColor(Vector3 ambientColor){ this->ambientColor = ambientColor; }
void Light::setDiffuseColor(Vector3 diffuseColor){ this->diffuseColor = diffuseColor; }
void Light::setSpecularColor(Vector3 specularColor){ this->specularColor = specularColor; }

void Light::Render(Material* material)
{
	//Using a string just for easy concatanation 
	//std::string lightString = "lights[";
	//lightString.append(std::to_string(lightIndex));
	//lightString.append("].");

	//Faster than a string
	char lightString[11];
	memcpy(lightString, "lights[", 7);
	_itoa(lightIndex, lightString + 7, 10);
	memcpy(lightString + 8, "].\0", 3);

	char posLightString[19];
	memcpy(posLightString, lightString, 10);
	memcpy(posLightString + 10, "position\0", 9);

	char ambLightString[23];
	memcpy(ambLightString, lightString, 10);
	memcpy(ambLightString + 10, "ambientColor\0", 13);

	char diffLightString[23];
	memcpy(diffLightString, lightString, 10);
	memcpy(diffLightString + 10, "diffuseColor\0", 13);

	char specLightString[24];
	memcpy(specLightString, lightString, 10);
	memcpy(specLightString + 10, "specularColor\0", 14);

	//Make sure to let the material know how many lights there are
	material->setInt("lightCount", LightCount);

	//Send light data
	material->setVector3(posLightString, position);
	material->setVector3(ambLightString, ambientColor);
	material->setVector3(diffLightString, diffuseColor);
	material->setVector3(specLightString, specularColor);

}

Light::~Light(void)
{
}
