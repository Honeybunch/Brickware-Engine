#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/DirectionalLightInternal.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

void DirectionalLightInternal::InitGL() {
  
}

void DirectionalLightInternal::RenderShadowMapGL()
{

}

void DirectionalLightInternal::BindShadowMapGL(Shader *shader) {
  shader->setGlobalMatrix4("depthBiasVP", depthBiasVP);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, renderTexture->depthBufferTexture);
}
