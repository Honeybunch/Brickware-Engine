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
	Spawner(GameObject* gameObject);

	virtual void Update() override;

	~Spawner();

private:
	GameObject* spawnable;

	void spawnObject();
};

#endif