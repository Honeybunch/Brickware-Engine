#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Texture.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;

#ifdef D3D_SUPPORT

void Texture::bufferD3D()
{
	//Create a description of the texture
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;

	//Set texture format based on the recorded texture format
	if (textureType == TextureType::RGBA)
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	//Setup the subresource data (pixel data)
	D3D11_SUBRESOURCE_DATA pixelData;
	pixelData.pSysMem = (void*)pixels;
	pixelData.SysMemPitch = width * 4;
	pixelData.SysMemSlicePitch = height * width * 4;

	HR(RenderingManager::device->CreateTexture2D(&textureDesc, &pixelData, &d3dTexture));

	//Create a description of the resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MipLevels = 1;

	//Create the SRV
	HR(RenderingManager::device->CreateShaderResourceView(d3dTexture, nullptr, &d3dTextureSRV));

	//Clear out descriptions
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	ZeroMemory(&pixelData, sizeof(pixelData));
	ZeroMemory(&resourceDesc, sizeof(resourceDesc));
}

void Texture::bindD3D()
{
	RenderingManager::deviceContext->PSSetShaderResources(0, 1, &d3dTextureSRV);
}

void Texture::freeD3D()
{
	//Game::deviceContext->PSSetShaderResources(0, 1, 0);
}

#endif