#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics\RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;

//Statics
Material* RenderingManager::currentMaterial;
std::vector<Renderable> RenderingManager::renderables;
std::vector<Light*> RenderingManager::lights;

#ifdef D3D_SUPPORT
ID3D11Device* RenderingManager::device;
ID3D11DeviceContext* RenderingManager::deviceContext;
#endif

void RenderingManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
#ifdef D3D_SUPPORT
	RenderingManager::device = device;
	RenderingManager::deviceContext = deviceContext;
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
	//Render every renderable object
	for (unsigned int i = 0; i < renderables.size(); i++)
	{
		//Send light data to the renderable material
		for (unsigned int j = 0; j < lights.size(); j++)
		{
			lights[j]->Render(renderables[i].material);
		}

#ifdef GL_SUPPORT
		RenderGL(renderables[i]);
#endif
#ifdef D3D_SUPPORT
		RenderD3D(renderables[i]);
#endif
	}

	lights.clear();
	renderables.clear();
}

void RenderingManager::Destroy()
{

}