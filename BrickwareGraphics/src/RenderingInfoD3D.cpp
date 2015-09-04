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
	// Check version based on feature support
	// Details here: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476124%28v=vs.85%29.aspx

	if (RenderingManager::device->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS1, 0, 0) == S_OK)
		return "11.2";
	else if (RenderingManager::device->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS, 0, 0) == S_OK)
		return "11.1";
	else
		return "11.0";
}
int RendererInfo::GetD3DMajorVersion()
{
	//We're always working with DX11 in this engine
	return 11;
}
int RendererInfo::GetD3DMinorVersion()
{
	// Check version based on feature support
	// Details here: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476124%28v=vs.85%29.aspx

	if (RenderingManager::device->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS1, 0, 0) == S_OK)
		return 2;
	else if (RenderingManager::device->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS, 0, 0) == S_OK)
		return 1;
	else
		return 0;
}

std::string RendererInfo::GetD3DVendor()
{
	DXGI_ADAPTER_DESC adapterDesc;
	RenderingManager::dxgiAdapter->GetDesc(&adapterDesc);

	// We just get an ID not a string as to the Chipset vendor
	// Use this list: http://www.pcidatabase.com/vendors.php?sort=id

	switch (adapterDesc.VendorId)
	{
	case 0x10DE:
		return "NVIDIA";
	case 0x1002:
		return "Advanced Micro Devices, Inc.";
	case 0x1022:
		return "Advanced Micro Devices";
	case 0x163C:
		return "intel";
	case 0x8086:
		return "Intel Corporation";
	case 0x8087:
		return "Intel";
	default:
		return std::to_string(adapterDesc.VendorId);
	}
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