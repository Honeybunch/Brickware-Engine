#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore\MeshRenderer.hpp"
#include "BrickwareCore\Softbody.hpp"

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

//Determine if there is any chance that we'll need to rebuffer the mesh
void MeshRenderer::Start()
{
	GameObject* gameObject = getGameObject();
	if (gameObject->getComponent<Softbody>() != NULL)
		mesh->setBufferHint(BufferHint::DYNAMIC_DRAW);
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