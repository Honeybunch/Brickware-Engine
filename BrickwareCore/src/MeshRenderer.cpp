#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/MeshRenderer.hpp"
#include "BrickwareCore/GameObject.hpp"

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
{
	if (castsShadows)
		this->shadowMaterial = new Material(RenderingManager::ShadowShader);

	this->material = new Material(*material);
	this->mesh = mesh;
}

Mesh* MeshRenderer::getMesh(){ return mesh; }
Material* MeshRenderer::getMaterial(){ return material; }

Material* MeshRenderer::getShadowMaterial()
{
	if (shadowMaterial != nullptr)
		return shadowMaterial;
	else
		return nullptr;
}

Bounds MeshRenderer::getBounds(){ return mesh->getBounds(); }

Component* MeshRenderer::Clone()
{ 
	MeshRenderer* copy = new MeshRenderer(*this);
	copy->material = new Material(*material);
	copy->shadowMaterial = new Material(*shadowMaterial);
	return copy; 
}

//Determine if there is any chance that we'll need to rebuffer the mesh
void MeshRenderer::Start()
{

}

//Draw everything in the VBOs
void MeshRenderer::Render()
{	
	RenderingManager::UseMaterial(material);
	RenderingManager::UseShadowMaterial(shadowMaterial);
	RenderingManager::DrawMesh(mesh);
}

MeshRenderer::~MeshRenderer()
{
}
