#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "BrickwareCoreDLL.h"

#include <unordered_map>

#include "Settings.h"
#include "Rigidbody.h"
#include "Collider.h"

class PhysicsManager
{
	friend class Game;
	friend class Rigidbody;
	friend class Collider;

public: 
	static BRICKWARE_CORE_API float GetGravity();
	static BRICKWARE_CORE_API void SetGravity(float gravity);

private:
	static void Initialize();
	static void Update();
	static void Destroy();
	
	static void AddRigidbody(Rigidbody* rigidbody);
	static void AddCollider(Collider* collider);

	static void RemoveRigidbody(Rigidbody* rigidbody);
	static void RemoveCollider(Collider* collider);

	//Using unordered map for faster removals
	static std::unordered_map<Rigidbody*, int> rigidbodies;
	static std::unordered_map<Collider*, int> colliders;
	static float gravity;
};

#endif