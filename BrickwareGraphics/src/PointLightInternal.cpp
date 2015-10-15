#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/PointLightInternal.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

int PointLightInternal::LightCount;

PointLightInternal::PointLightInternal() : Light()
{
	position = Vector3(0,0,0);

	//Note the index of this light
	lightIndex = LightCount;
	LightCount++;

	RenderingAPI renderer = GraphicsSettings::Renderer;

	//Initialize based on rendering API
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			InitPtr = &PointLightInternal::InitGL;
			if (GraphicsSettings::Shadows)
			{
				RenderShadowMapPtr = &PointLightInternal::RenderShadowMapGL;
				BindShadowMapPtr = &PointLightInternal::BindShadowMapGL;
			}
		}
		else
		{
			std::cout << "Your card does not support OpenGL 3+" << std::endl;
		}
	}

	farPlane = 30.0f;

	Init();
}

void PointLightInternal::setPosition(Vector3 position)
{
	this->position = position;
}

void PointLightInternal::RenderLight(Shader* shader)
{
	//Using a string just for easy concatanation 
	std::string lightString = "pointLights[";
	lightString.append(std::to_string(lightIndex));
	lightString.append("].");
	
	std::string posLightString = lightString + "position";

	std::string ambLightString = lightString + "ambientColor";
	std::string diffLightString = lightString + "diffuseColor";
	std::string specLightString = lightString + "specularColor";


	//Make sure to let the material know how many lights there are
	shader->setGlobalInt("pointLightCount", LightCount);

	//Send light data
	shader->setGlobalVector3(posLightString.c_str(), position);
	shader->setGlobalVector3(ambLightString.c_str(), ambientColor);
	shader->setGlobalVector3(diffLightString.c_str(), diffuseColor);
	shader->setGlobalVector3(specLightString.c_str(), specularColor);
}

void PointLightInternal::Init()
{
	(this->*InitPtr)();
}

void PointLightInternal::RenderShadowMap()
{
	//Render cube map
	(this->*RenderShadowMapPtr)();
}

void PointLightInternal::BindShadowMap(Shader* shader)
{
	//Bind Shadow cube map
	(this->*BindShadowMapPtr)(shader);
}

PointLightInternal::~PointLightInternal()
{

}