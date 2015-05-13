#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Material.hpp"

#ifdef D3D_SUPPORT

using namespace Brickware;
using namespace Graphics;
using namespace Math;

std::vector<ID3D11Buffer*> Material::getConstantBuffers(){ return shader->constantBuffers; }
std::vector<char*> Material::getConstantBufferData(){ return shader->constantBufferData; }

#endif