#ifdef D3D_SUPPORT

#define BRICKWARE_GRAPHICS_EXPORTS

#include <dxgi.h>
#include <d3d11.h>

#include "BrickwareGraphics/RenderingManager.hpp"
#include "BrickwareGraphics/RendererInfo.hpp"

using namespace Brickware;
using namespace Graphics;

std::string RendererInfo::GetD3DVersion()
{
	return "";
}
int RendererInfo::GetD3DMajorVersion()
{
	return 0;
}
int RendererInfo::GetD3DMinorVersion()
{
	return 0;
}

std::string RendererInfo::GetD3DVendor()
{
	DXGI_ADAPTER_DESC adapterDesc;
	RenderingManager::dxgiAdapter->GetDesc(&adapterDesc);

	return "";
}

std::string RendererInfo::GetD3DRenderer()
{
	DXGI_ADAPTER_DESC adapterDesc;
	RenderingManager::dxgiAdapter->GetDesc(&adapterDesc);

	std::wstring wideDesc = std::wstring((wchar_t*)adapterDesc.Description);
	return std::string(wideDesc.begin(), wideDesc.end());
}

std::string RendererInfo::GetHLSLVersion()
{
	return "";
}
std::vector<std::string> RendererInfo::GetAllSupportedHLSLVersions()
{
	std::vector<std::string> supportedHLSLVersions;
	return supportedHLSLVersions;
}

#endif