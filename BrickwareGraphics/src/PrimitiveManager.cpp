#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/PrimitiveManager.hpp"
#include "BrickwareGraphics/Primitive.hpp"

using namespace Brickware;
using namespace Graphics;

std::vector<Primitive*> PrimitiveManager::Primitives;
Shader* PrimitiveManager::shader;

//Static function pointers
void(*PrimitiveManager::DrawPrimitivePtr)(Primitive* p);
void(*PrimitiveManager::BufferDataPtr)();
void(*PrimitiveManager::DestroyDataPtr)();

//Used to load the basic shader and buffer data
void PrimitiveManager::Initialize()
{
	shader = new Shader("Shaders/PrimitiveVertex", "Shaders/PrimitivePixel");

	//Setup function pointers based on rendering API
	RenderingAPI renderer = GraphicsSettings::Renderer;
#ifdef GL_SUPPORT
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			DrawPrimitivePtr = &PrimitiveManager::DrawPrimitiveGL;
			BufferDataPtr = &PrimitiveManager::BufferDataGL;
			DestroyDataPtr = &PrimitiveManager::DestroyDataGL;
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
			DrawPrimitivePtr = &PrimitiveManager::DrawPrimitiveD3D;
			BufferDataPtr = &PrimitiveManager::BufferDataD3D;
			DestroyDataPtr = &PrimitiveManager::DestroyDataD3D;
		}
}
#endif

	//We want to create basic buffers of our few basic shapes
	(*BufferDataPtr)();
}

void PrimitiveManager::ClearPrimitives()
{
	for (unsigned int i = 0; i < Primitives.size(); i++)
		delete Primitives[i];

	Primitives.clear();
}

void PrimitiveManager::DrawPrimitives(Math::Matrix4 viewMatrix, Math::Matrix4 projectionMatrix)
{
	shader->bindShader();

	for (unsigned int i = 0; i < Primitives.size(); i++)
	{
		Primitive* p = Primitives[i];
		p->worldMatrix = (p->modelMatrix * viewMatrix) * projectionMatrix;

		//Draw with proper APIs
		(*DrawPrimitivePtr)(p);
	}
	shader->freeShader();
}

void PrimitiveManager::Destroy()
{
	ClearPrimitives();

	(*DestroyDataPtr)();

	delete shader;
}


#ifdef D3D_SUPPORT

void PrimitiveManager::BufferDataD3D()
{
	
}

#endif