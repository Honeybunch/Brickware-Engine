#define BRICKWARE_CORE_EXPORTS

#include "MeshRenderer.h"

using namespace Brickware;
using namespace Core;
using namespace Graphics;

MeshRenderer::MeshRenderer(Mesh* mesh)
{
	this->mesh = mesh;
}

Bounds MeshRenderer::getBounds()
{
	return mesh->getBounds();
}

Component* MeshRenderer::Clone(){ return new MeshRenderer(*this); }

//Draw everything in the VBOs
void MeshRenderer::Render()
{	
	RenderingManager::DrawMesh(mesh);
}

MeshRenderer::~MeshRenderer()
{
}