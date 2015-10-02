#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/PointLightInternal.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

void PointLightInternal::InitGL()
{

}

void PointLightInternal::RenderShadowMapGL(Shader* shadowShader)
{
	//Render cube map
}

void PointLightInternal::BindShadowMapGL(Shader* shader)
{
	//Bind Shadow cube map
}