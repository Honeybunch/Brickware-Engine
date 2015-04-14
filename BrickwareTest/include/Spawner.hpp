#ifndef SPAWNER_H
#define SPAWNER_H

#include "Component.h"
#include "Material.h"
#include "Shader.h"
#include "SphereCollider.h"
#include "Vector3.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Rigidbody.h"
#include "Despawn.h"

class Spawner :
	public Brickware::Core::Component
{
public:
	Spawner(Brickware::Core::GameObject* gameObject);

	virtual void Update() override;

	~Spawner();

private:
	Brickware::Core::GameObject* spawnable;
	bool keyDown;

	void spawnObject();
};

#endif