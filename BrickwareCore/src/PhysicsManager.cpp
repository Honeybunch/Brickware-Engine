#define BRICKWARE_CORE_EXPORTS

#include "PhysicsManager.h"

//Statics
std::vector<Collision*> PhysicsManager::activeCollisions;
std::vector<Collision*> PhysicsManager::lastFrameActiveCollisions;
std::unordered_map<Rigidbody*, int> PhysicsManager::rigidbodies;
std::unordered_map<Collider*, int> PhysicsManager::colliders;
float PhysicsManager::gravity;

float PhysicsManager::GetGravity(){ return gravity; }
void PhysicsManager::SetGravity(float gravity){ PhysicsManager::gravity = gravity; }

void PhysicsManager::Initialize()
{
	gravity = -.004905f;
}

bool PhysicsManager::IsCollisionActive(Collision* collision)
{
	for (unsigned int i = 0; i < activeCollisions.size(); i++)
	{
		Collision* activeCollision = activeCollisions[i];

		if (activeCollision->getThisObject() == collision->getThisObject() &&
			activeCollision->getOtherObject() == collision->getOtherObject())
			return true;
	}

	for (unsigned int i = 0; i < lastFrameActiveCollisions.size(); i++)
	{
		Collision* activeCollision = lastFrameActiveCollisions[i];

		if (activeCollision->getThisObject() == collision->getThisObject() &&
			activeCollision->getOtherObject() == collision->getOtherObject())
			return true;
	}

	return false;
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

					//If there isn't any identical collision, we will send a collision enter call
					if (IsCollisionActive(collision) == false)
					{
						testObj->OnCollisionEnter(collision);

						activeCollisions.push_back(collision);
					}
					//Otherwise we will send a collision continue call
					else
					{
						//TODO
					}
				}
			}
		}
	}

	//Any collisions that were active last frame that are not active now can be untracked
	for (unsigned int i = 0; i < lastFrameActiveCollisions.size(); i++)
	{
		Collision* toCheck = lastFrameActiveCollisions[i];
		bool shouldRemove = true;
		for (unsigned int j = 0; j < activeCollisions.size(); j++)
		{
			if (toCheck == activeCollisions[j])
			{
				shouldRemove = false;
				break;
			}
		}
		if (shouldRemove)
		{
			lastFrameActiveCollisions.erase(lastFrameActiveCollisions.begin() + i, lastFrameActiveCollisions.begin() + i + 1);
			//TODO: fire off collision end event
		}
	}

	//Any active collisions are stored 
	for (unsigned int i = 0; i < activeCollisions.size(); i++)
		lastFrameActiveCollisions.push_back(activeCollisions[i]);
	
	activeCollisions.clear();
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
