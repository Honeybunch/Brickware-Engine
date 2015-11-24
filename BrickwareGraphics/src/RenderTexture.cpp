#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/RenderTexture.hpp"

using namespace Brickware;
using namespace Graphics;

/*
	Public
*/

RenderTexture::RenderTexture(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	this->depth = 16;
	this->format = TextureFormat::RGBA;

	Init();
}

RenderTexture::RenderTexture(unsigned int width, unsigned int height, unsigned int depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->format = TextureFormat::RGBA;

	Init();
}

RenderTexture::RenderTexture(unsigned int width, unsigned int height, unsigned int depth, TextureFormat format)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->format = format;

	Init();
}

void RenderTexture::Bind()
{
	(this->*BindPtr)();
}

void RenderTexture::Free()
{
	(this->*FreePtr)();
}

RenderTexture::~RenderTexture()
{
	(this->*DestroyPtr)();
}

/*
	Private
*/

void RenderTexture::Init()
{
	RenderingAPI renderer = GraphicsSettings::Renderer;

	//Setup function pointers based on rendering API

#ifdef GL_SUPPORT
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			InitGL();

			BindPtr = &RenderTexture::BindGL;
			FreePtr = &RenderTexture::FreeGL;
		}
		else
		{
			std::cout << "Error loading Shader: Your card does not support OpenGL 3+" << std::endl;
		}
	}
#endif

#ifdef D3D_SUPPORT
	if (renderer = RenderingAPI::DirectX)
	{
		if (RendererInfo::GetAPIMajorVersion() == 11)
		{
			InitD3D();

			BindPtr = &RenderTexture::BindD3D;
			FreePtr = &RenderTexture::FreeD3D;
		}
	}
#endif
}