#define BRICKWARE_GRAPHICS_EXPORTS

#include "RenderingManager.h"

using namespace Brickware;
using namespace Graphics;

//Statics
Material* RenderingManager::currentMaterial;
std::vector<Renderable> RenderingManager::renderables;
std::vector<Light*> RenderingManager::lights;

void RenderingManager::Initialize()
{
	
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
	//Send light data to all materials
	std::vector<Material*> materials = Material::Materials;
	for (unsigned int i = 0; i < materials.size(); i++)
	{
		Material* material = materials[i];
		for (unsigned int j = 0; j < lights.size(); j++)
		{
			lights[j]->Render(material);
		}
	}

	//Render every renderable object
	for (unsigned int i = 0; i < renderables.size(); i++)
	{
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