#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;

//Statics
Material* RenderingManager::currentMaterial;
std::vector<Renderable> RenderingManager::renderables;
std::vector<Light*> RenderingManager::lights;

#ifdef D3D_SUPPORT
ID3D11Device* RenderingManager::device;
ID3D11DeviceContext* RenderingManager::deviceContext;
IDXGIDevice* RenderingManager::dxgiDevice;
IDXGIAdapter* RenderingManager::dxgiAdapter;
#endif

void RenderingManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
#ifdef D3D_SUPPORT
	RenderingManager::device = device;
	RenderingManager::deviceContext = deviceContext;
	
	RenderingManager::device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	RenderingManager::dxgiDevice->GetAdapter(&dxgiAdapter);
#endif
}

void RenderingManager::AddLight(Light* light)
{
	lights.push_back(light);
}
void RenderingManager::UseMaterial(Material* material)
{
	currentMaterial = material;
}
void RenderingManager::DrawMesh(Mesh* mesh)
{
	Renderable renderable;
	renderable.mesh = mesh;
	renderable.material = currentMaterial;

	renderables.push_back(renderable);
}

void RenderingManager::Render()
{
	Shader* activeShader = nullptr;

	//Render every renderable object
	for (unsigned int i = 0; i < renderables.size(); i++)
	{
		Renderable renderable = renderables[i];

		if (renderable.material->shader != activeShader)
		{
			if (activeShader != nullptr)
				activeShader->freeShader();
			activeShader = renderable.material->shader;

			activeShader->bindShader();

			//Send light data to the shader
			for (unsigned int j = 0; j < lights.size(); j++)
			{
				lights[j]->Render(activeShader);
			}
		}

#ifdef GL_SUPPORT
		RenderGL(renderable);
#endif
#ifdef D3D_SUPPORT
		RenderD3D(renderable);
#endif
	}

	lights.clear();
	renderables.clear();
}

void RenderingManager::Destroy()
{

}