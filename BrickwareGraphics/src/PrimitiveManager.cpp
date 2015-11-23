#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/PrimitiveManager.hpp"
#include "BrickwareGraphics/Primitive.hpp"

using namespace Brickware;
using namespace Graphics;

std::vector<Primitive*> PrimitiveManager::PointPrimitives;
std::vector<Primitive*> PrimitiveManager::LinePrimitives;
Shader* PrimitiveManager::pointShader;
Shader* PrimitiveManager::lineShader;

//Static function pointers
void(*PrimitiveManager::DrawPrimitivePtr)(Primitive* p, Shader* shader);
void(*PrimitiveManager::BufferDataPtr)();
void(*PrimitiveManager::DestroyDataPtr)();

//Used to load the basic shader and buffer data
void PrimitiveManager::Initialize()
{
	pointShader = new Shader("Shaders/PrimitivePointGeometry", "Shaders/PrimitiveVertex", "Shaders/PrimitivePixel");
	lineShader = new Shader("Shaders/PrimitiveVertex", "Shaders/PrimitivePixel");

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
	for (unsigned int i = 0; i < PointPrimitives.size(); i++)
		delete PointPrimitives[i];

	PointPrimitives.clear();

	for (unsigned int i = 0; i < LinePrimitives.size(); i++)
		delete LinePrimitives[i];

	LinePrimitives.clear();
}

void PrimitiveManager::DrawPrimitives(Math::Matrix4 viewMatrix, Math::Matrix4 projectionMatrix)
{
	pointShader->bindShader();

	for (unsigned int i = 0; i < PointPrimitives.size(); i++)
	{
		Primitive* p = PointPrimitives[i];
		p->worldMatrix = (p->modelMatrix * viewMatrix) * projectionMatrix;

		//Draw with proper APIs
		(*DrawPrimitivePtr)(p, pointShader);
	}

	pointShader->freeShader();

	lineShader->bindShader();

	for (unsigned int i = 0; i < LinePrimitives.size(); i++)
	{
		Primitive* p = LinePrimitives[i];
		p->worldMatrix = (p->modelMatrix * viewMatrix) * projectionMatrix;

		//Draw with proper APIs
		(*DrawPrimitivePtr)(p, lineShader);
	}

	lineShader->freeShader();
}

void PrimitiveManager::Destroy()
{
	ClearPrimitives();

	if (DestroyDataPtr)
		(*DestroyDataPtr)();

	if (pointShader)
		delete pointShader;
	if (lineShader)
		delete lineShader;
}


#ifdef D3D_SUPPORT

void PrimitiveManager::BufferDataD3D()
{
	
}

#endif