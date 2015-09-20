#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/PointLight.hpp"
#include "BrickwareCore/GameObject.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;

PointLight::PointLight()
{
	light = new Graphics::PointLightInternal();
}

void PointLight::setDiffuseColor(Vector3 diffuseColor){ light->setDiffuseColor(diffuseColor); }
void PointLight::setSpecularColor(Vector3 specularColor){ light->setSpecularColor(specularColor); }

void PointLight::Render()
{
	light->setPosition(getGameObject()->getTransform()->getPosition());
	Graphics::RenderingManager::AddPointLight(light);
}

#ifdef _DEBUG
void PointLight::DebugDraw()
{
	Graphics::Primitive::SetColor(Vector4(1, 1, 0, 1));
	Graphics::Primitive::SetPointSize(20.0f);

	Graphics::Primitive::DrawPoint(getGameObject()->getTransform()->getPosition());
}
#endif

PointLight::~PointLight()
{
	delete light;
}