#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore\PointLight.hpp"
#include "BrickwareCore\GameObject.hpp"

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;

PointLight::PointLight()
{
	light = new Light();
}

void PointLight::setDiffuseColor(Math::Vector3 diffuseColor){ light->setDiffuseColor(diffuseColor); }
void PointLight::setSpecularColor(Math::Vector3 specularColor){ light->setSpecularColor(specularColor); }

void PointLight::Render()
{
	light->setPosition(getGameObject()->getTransform()->getPosition());
	RenderingManager::AddLight(light);
}

#ifdef _DEBUG
void PointLight::DebugDraw()
{
	Primitive::SetColor(Vector4(1, 1, 0, 1));
	Primitive::SetPointSize(20.0f);

	Primitive::DrawPoint(getGameObject()->getTransform()->getPosition());
}
#endif

PointLight::~PointLight()
{
	delete light;
}