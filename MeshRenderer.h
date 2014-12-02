#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Mesh.h"
#include "Component.h"


class MeshRenderer : public Component
{
public:
	MeshRenderer(Mesh* mesh);

	virtual void Render();

	~MeshRenderer();

private:
	Mesh* mesh;
};

#endif