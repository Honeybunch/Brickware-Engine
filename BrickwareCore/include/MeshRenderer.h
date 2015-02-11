#ifndef MESHRENDERER_H
#define MESHRENDERER_H

//Only use this line to make sure VS creates an import lib
#ifdef BRICKWARE_CORE_EXPORTS
#define BRICKWARE_CORE_API __declspec(dllexport)
#else
#define BRICKWARE_CORE_API __declspec(dllimport)
#endif

#include "Mesh.h"
#include "Component.h"
#include "Bounds.h"

class BRICKWARE_CORE_API MeshRenderer : public Component
{
public:
	MeshRenderer(Mesh* mesh);

	Bounds* getBounds();

	virtual void Update();
	virtual void Render();

	~MeshRenderer();

private:
	Mesh* mesh;
	Bounds* bounds;

	void calculateBounds();

	void renderGL(Material* material);
	void renderD3D(Material* material);
};

#endif