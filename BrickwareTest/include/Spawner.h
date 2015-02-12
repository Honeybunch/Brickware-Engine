#ifndef SPAWNER_H
#define SPAWNER_H

#include "Component.h"
#include "Material.h"
#include "Shader.h"
#include "SphereCollider.h"
#include "Vector3.h"
#include "MeshRenderer.h"
#include "Camera.h"

class Spawner :
	public Component
{
public:
	Spawner(Mesh* mesh, Shader* shader);

	virtual void Update() override;

	~Spawner();

private:
	Mesh* mesh;
	Shader* shader;

	void spawnSphere();
};

#endif