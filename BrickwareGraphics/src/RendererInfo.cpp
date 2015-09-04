#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/RendererInfo.hpp"

using namespace Brickware;
using namespace Graphics;

std::string RendererInfo::GetAPIVersion()
{
#ifdef GL_SUPPORT
	return RendererInfo::GetGLVersion();
#elif D3D_SUPPORT
	return RendererInfo::GetD3DVersion();
#endif
}
int RendererInfo::GetAPIMajorVersion()
{
#ifdef GL_SUPPORT
	return RendererInfo::GetGLMajorVersion();
#elif D3D_SUPPORT
	return RendererInfo::GetD3DMajorVersion();
#endif
}
int RendererInfo::GetAPIMinorVersion()
{
#ifdef GL_SUPPORT
	return RendererInfo::GetGLMinorVersion();
#elif D3D_SUPPORT
	return RendererInfo::GetD3DMinorVersion();
#endif
}

std::string RendererInfo::GetVendor()
{
#ifdef GL_SUPPORT
	return RendererInfo::GetGLVendor();
#elif D3D_SUPPORT
	return RendererInfo::GetD3DVendor();
#endif
}

std::string RendererInfo::GetRenderer()
{
#ifdef GL_SUPPORT
	return RendererInfo::GetGLRenderer();
#elif D3D_SUPPORT
	return RendererInfo::GetD3DRenderer();
#endif
}

std::string RendererInfo::GetShadingLanguageVersion()
{
#ifdef GL_SUPPORT
	return RendererInfo::GetGLSLVersion();
#elif D3D_SUPPORT
	return RendererInfo::GetHLSLVersion();
#endif
}
std::vector<std::string> RendererInfo::GetAllSupportedShadingLanguageVersions()
{
#ifdef GL_SUPPORT
	return RendererInfo::GetAllSupportedGLSLVersions();
#elif D3D_SUPPORT
	return RendererInfo::GetAllSupportedHLSLVersions();
#endif
}