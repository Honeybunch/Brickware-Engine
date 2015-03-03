#define BRICKWARE_CORE_EXPORTS

#include "PhysicsManager.h"

//Statics
std::unordered_map<Rigidbody*, int> PhysicsManager::rigidbodies;
//std::unordered_map<Collider*, Collider> PhysicsManager::colliders;
float PhysicsManager::gravity;

float PhysicsManager::GetGravity(){ return gravity; }
void PhysicsManager::SetGravity(float gravity){ PhysicsManager::gravity = gravity; }

void PhysicsManager::Initialize()
{
	gravity = -.0981f;
}

void PhysicsManager::Update()
{
	//Rigidbodies will be updated in their fixed update methods
	
	for (std::pair<Rigidbody*, int> pair : rigidbodies)
	{		
		Rigidbody* rigidbody = pair.first;
		rigidbody->FixedUpdate();

		//Determine if colliders are colliding
		

		//Dispatch to colliding game object componets

	}	
}

void PhysicsManager::Destroy()
{

}

void PhysicsManager::AddRigidbody(Rigidbody* rigidbody)
{
	rigidbodies[rigidbody] = 0;
}
/*
void PhysicsManager::AddCollider(Collider* collider)
{
	colliders[collider] = *collider;
}
*/

void PhysicsManager::RemoveRigidbody(Rigidbody* rigidbody)
{
	rigidbodies.erase(rigidbody);
}
/*
void PhysicsManager::RemoveCollider(Collider* collider)
{
	colliders.erase(collider);
}
*/