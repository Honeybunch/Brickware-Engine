#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/DirectionalLightInternal.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

Shader* DirectionalLightInternal::ShadowShader = nullptr;

DirectionalLightInternal::DirectionalLightInternal() : Light()
{
	direction = Vector3(0, -1, 0);

	RenderingAPI renderer = GraphicsSettings::Renderer;

	//Initialize based on rendering API
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			InitGL();
			if (GraphicsSettings::Shadows)
			{
				InternalRender = &DirectionalLightInternal::RenderGL;
				if (ShadowShader == nullptr)
				{
					ShadowShader = new Shader("Shaders/PrimitiveVertex", "Shaders/ShadowPixel");
				}
			}
			else
			{
				InternalRender = &DirectionalLightInternal::BasicRender;
			}
		}
		else
		{
			std::cout << "Your card does not support OpenGL 3+" << std::endl;
		}
	}
	
}

void DirectionalLightInternal::setDirection(Vector3 direction)
{
	this->direction = direction;
}

void DirectionalLightInternal::Render(Shader* shader)
{
	(this->*InternalRender)(shader);
}

void DirectionalLightInternal::BasicRender(Shader* shader)
{
	std::string baseString = "directionalLight.";

	std::string dirLightString = baseString + "direction";

	std::string ambLightString = baseString + "ambientColor";
	std::string diffLightString = baseString + "diffuseColor";
	std::string specLightString = baseString + "specularColor";

	//Send light data
	shader->setGlobalVector3(dirLightString.c_str(), direction);
	shader->setGlobalVector3(ambLightString.c_str(), ambientColor);
	shader->setGlobalVector3(diffLightString.c_str(), diffuseColor);
	shader->setGlobalVector3(specLightString.c_str(), specularColor);
}

DirectionalLightInternal::~DirectionalLightInternal()
{

}