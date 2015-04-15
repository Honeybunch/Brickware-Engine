#ifndef SPAWNER_H
#define SPAWNER_H

#include "BrickwareCore\Component.hpp"
#include "BrickwareCore\SphereCollider.hpp"
#include "BrickwareCore\MeshRenderer.hpp"
#include "BrickwareCore\Camera.hpp"
#include "BrickwareCore\Rigidbody.hpp"

#include "BrickwareGraphics\Material.hpp"
#include "BrickwareGraphics\Shader.hpp"

#include "BrickwareMath\Vector3.hpp"

#include "Despawn.hpp"

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