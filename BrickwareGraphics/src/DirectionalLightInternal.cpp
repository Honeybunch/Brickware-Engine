#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/DirectionalLightInternal.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

DirectionalLightInternal::DirectionalLightInternal() : Light()
{
	direction = Vector3(0, -1, 0);
}

void DirectionalLightInternal::setDirection(Vector3 direction)
{
	this->direction = direction;
}

void DirectionalLightInternal::Render(Shader* shader)
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