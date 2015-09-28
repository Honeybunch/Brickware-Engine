#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/DirectionalLight.hpp"
#include "BrickwareCore/GameObject.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;

DirectionalLight::DirectionalLight()
{
	light = new Graphics::DirectionalLightInternal();
}

void DirectionalLight::setDirection(Vector3 direction){ light->setDirection(direction); }

void DirectionalLight::setDiffuseColor(Vector3 diffuseColor){ light->setDiffuseColor(diffuseColor); }
void DirectionalLight::setSpecularColor(Vector3 specularColor){ light->setSpecularColor(specularColor); }

void DirectionalLight::Render()
{
	Graphics::RenderingManager::AddLight(light);
}

#ifdef _DEBUG
void DirectionalLight::DebugDraw()
{
	Graphics::Primitive::SetColor(Vector4(1, 1, 0, 1));
	Graphics::Primitive::SetPointSize(20.0f);

	Graphics::Primitive::DrawPoint(getGameObject()->getTransform()->getPosition());
}
#endif

DirectionalLight::~DirectionalLight()
{
	delete light;
}
