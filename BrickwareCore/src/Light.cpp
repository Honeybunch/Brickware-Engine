#define BRICKWARE_CORE_EXPORTS

#include "light.h"

int Light::LightCount;

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

void Light::Start()
{
	lightCount = LightCount;
	LightCount++;
}

void Light::Render()
{
	//Using a string just for easy concatanation 
	std::string lightString = "lights[";
	lightString.append(std::to_string(lightCount));
	lightString.append("].");
	
	//Apply lights to every shader
	for (unsigned int i = 0; i < Material::Materials.size(); i++)
	{
		Material* material = Material::Materials[i];

		material->bindShader();

		//Make sure to let the material know how many lights there are
		material->setInt("lightCount", LightCount);

		//Send light data
		material->setVector3((lightString + "position").c_str(), getGameObject()->getTransform()->getPosition());
		material->setVector3((lightString + "ambientColor").c_str(), ambientColor);
		material->setVector3((lightString + "diffuseColor").c_str(), diffuseColor);
		material->setVector3((lightString + "specularColor").c_str(), specularColor);

		material->freeShader();
	}
}

#ifdef _DEBUG
void Light::DebugDraw()
{
	Primitive::SetColor(Vector4(1, 1, 0, 1));
	Primitive::SetPointSize(20.0f);

	Primitive::DrawPoint(getGameObject()->getTransform()->getPosition());
}
#endif

Light::~Light(void)
{
}
