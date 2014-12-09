#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Mesh.h"
#include "Component.h"
#include "Bounds.h"

class MeshRenderer : public Component
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
};

#endif