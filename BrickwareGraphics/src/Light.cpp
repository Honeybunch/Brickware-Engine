#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics\Light.hpp"

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
	std::string lightString = "lights[";
	lightString.append(std::to_string(lightIndex));
	lightString.append("].");

	//Make sure to let the material know how many lights there are
	material->setInt("lightCount", LightCount);

	//Send light data
	material->setVector3((lightString + "position").c_str(), position);
	material->setVector3((lightString + "ambientColor").c_str(), ambientColor);
	material->setVector3((lightString + "diffuseColor").c_str(), diffuseColor);
	material->setVector3((lightString + "specularColor").c_str(), specularColor);
}

Light::~Light(void)
{
}
