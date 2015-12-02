#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/DirectionalLightInternal.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

DirectionalLightInternal::DirectionalLightInternal() : InternalLight()
{
	direction = Vector3(0, -1, 0);

	RenderingAPI renderer = GraphicsSettings::Renderer;

	renderTexture = new RenderTexture(shadowMapRes, shadowMapRes, 16);

	//Initialize based on rendering API
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			InitPtr = &DirectionalLightInternal::InitGL;
			if (GraphicsSettings::Shadows)
			{
				RenderShadowMapPtr = &DirectionalLightInternal::RenderShadowMapGL;
				BindShadowMapPtr = &DirectionalLightInternal::BindShadowMapGL;
			}
		}
		else
		{
			std::cout << "Your card does not support OpenGL 3+" << std::endl;
		}
	}

	Init();
}

void DirectionalLightInternal::setDirection(Vector3 direction)
{
	this->direction = direction;
}

void DirectionalLightInternal::RenderLight(Shader* shader)
{
	std::string baseString = "directionalLight.";

	std::string dirLightString = baseString + "direction";

	std::string ambLightString = baseString + "ambientColor";
	std::string diffLightString = baseString + "diffuseColor";
	std::string specLightString = baseString + "specularColor";

	std::string shadowStrengthString = baseString + "shadowStrength";
	std::string shadowBiasString = baseString + "shadowBias";

	//Send light data
	shader->setGlobalVector3(dirLightString.c_str(), direction);
	shader->setGlobalVector3(ambLightString.c_str(), ambientColor);
	shader->setGlobalVector3(diffLightString.c_str(), diffuseColor);
	shader->setGlobalVector3(specLightString.c_str(), specularColor);

	shader->setGlobalFloat(shadowStrengthString.c_str(), shadowStrength);
	shader->setGlobalFloat(shadowBiasString.c_str(), shadowBias);
}

void DirectionalLightInternal::Init()
{
	(this->*InitPtr)();
}

void DirectionalLightInternal::RenderShadowMap()
{
	//Create render pass
	// Send info about the directional light to the shader for shadow mapping
	Vector3 focalPoint = direction * -30;

	// Compute MVP from light's direction
	Matrix4 depthProjection =
		Matrix4::getOrthographicProjection(-30, 30, -30, 30, -60, 60);
	Matrix4 depthView =
		Matrix4::getLookAtView(focalPoint, Vector3(0, 0, 0), Vector3(0, 1, 0));

	depthVP = depthView * depthProjection;

	Matrix4 biasMatrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	// Apply bias to get texture coordinates
	depthBiasVP = biasMatrix * depthVP;

	RenderPass shadowPass;
	shadowPass.view = depthView;
	shadowPass.projection = depthProjection;
	shadowPass.renderTexture = renderTexture;
	shadowPass.shader = RenderingManager::DirectionalShadowShader;

	RenderingManager::AddPreScenePass(shadowPass);
}

void DirectionalLightInternal::BindShadowMap(Shader* shader)
{
	(this->*BindShadowMapPtr)(shader);
}

DirectionalLightInternal::~DirectionalLightInternal()
{

}