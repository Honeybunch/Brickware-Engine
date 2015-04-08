#define BRICKWARE_CORE_EXPORTS

#include "MeshRenderer.h"

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
{
	this->material = new Material(*material);
	this->mesh = mesh;
}

Mesh* MeshRenderer::getMesh(){ return mesh; }
Material* MeshRenderer::getMaterial(){ return material; }
Bounds MeshRenderer::getBounds(){ return mesh->getBounds(); }

Component* MeshRenderer::Clone()
{ 
	MeshRenderer* copy = new MeshRenderer(*this);
	copy->material = new Material(*material);
	return copy; 
}

//Draw everything in the VBOs
void MeshRenderer::Render()
{	
	RenderingManager::UseMaterial(material);
	RenderingManager::DrawMesh(mesh);
}

MeshRenderer::~MeshRenderer()
{
}