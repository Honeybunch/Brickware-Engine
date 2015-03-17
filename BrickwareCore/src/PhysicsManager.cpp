#define BRICKWARE_CORE_EXPORTS

#include "PhysicsManager.h"

//Statics
std::unordered_map<Rigidbody*, int> PhysicsManager::rigidbodies;
std::unordered_map<Collider*, int> PhysicsManager::colliders;
float PhysicsManager::gravity;

float PhysicsManager::GetGravity(){ return gravity; }
void PhysicsManager::SetGravity(float gravity){ PhysicsManager::gravity = gravity; }

void PhysicsManager::Initialize()
{
	gravity = -.0004905f;
}

void PhysicsManager::Update()
{
	//Update rigidbodies
	for (std::pair<Rigidbody*, int> pair : rigidbodies)
	{		
		Rigidbody* rigidbody = pair.first;
		rigidbody->FixedUpdate();
	}	

	//Detect collisions
	for (auto it1 = colliders.begin(); it1 != colliders.end(); it1++)
	{
		Collider* test = it1->first;

		for (auto it2 = colliders.begin(); it2 != colliders.end(); it2++)
		{
			Collider* other = it2->first;

			bool colliding = false;
			Collision* collision = new Collision();
			if (other != test)
				colliding = test->isColliding(other, collision);

			if (other != test && colliding)
			{
				//Dispatch collision
				if (collision)
				{
					GameObject* testObj = test->getGameObject();

					testObj->OnCollisionEnter(collision);
				}
			}
		}
	}
}

void PhysicsManager::Destroy()
{

}

void PhysicsManager::AddRigidbody(Rigidbody* rigidbody)
{
	rigidbodies[rigidbody] = 0;
}
void PhysicsManager::AddCollider(Collider* collider)
{
	colliders[collider] = 0;
}

void PhysicsManager::RemoveRigidbody(Rigidbody* rigidbody)
{
	rigidbodies.erase(rigidbody);
}
void PhysicsManager::RemoveCollider(Collider* collider)
{
	colliders.erase(collider);
}
