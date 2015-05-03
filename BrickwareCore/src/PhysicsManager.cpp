#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore\PhysicsManager.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;

//Statics
std::vector<Collision*> PhysicsManager::activeCollisions;
std::vector<Collision*> PhysicsManager::lastFrameActiveCollisions;
std::unordered_map<Rigidbody*, int> PhysicsManager::rigidbodies;
std::unordered_map<Softbody*, int> PhysicsManager::softbodies;
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

	//Update softbodies
	for (std::pair<Softbody*, int> pair : softbodies)
	{
		Softbody* softbody = pair.first;
		softbody->FixedUpdate();
	}

	//Detect collisions
	for (auto it1 = colliders.begin(); it1 != colliders.end(); it1++)
	{
		Collider* test = it1->first;
		if (test->getGameObject()->getComponent<Rigidbody>() == NULL)
			continue;

		for (auto it2 = colliders.begin(); it2 != colliders.end(); it2++)
		{
			Collider* other = it2->first;
			if (other->getGameObject()->getComponent<Rigidbody>() == NULL)
				continue;

			bool colliding = false;
			Collision* collision = new Collision();
			Collision* oppositeCollision = new Collision();
			if (other != test)
			{
				colliding = test->isColliding(other, collision);
				other->isColliding(test, oppositeCollision);
			}

			if (other != test && colliding)
			{
				//Dispatch collision
				if (collision)
				{
					GameObject* testObj = test->getGameObject();
					GameObject* otherObj = other->getGameObject();

					//If there isn't any identical collision, we will send a collision enter call
					if (IsCollisionActive(collision) == false && IsCollisionActive(oppositeCollision) == false)
					{
						testObj->OnCollisionEnter(collision);
						otherObj->OnCollisionEnter(oppositeCollision);

						activeCollisions.push_back(collision);
						activeCollisions.push_back(oppositeCollision);
					}
					//Otherwise we will send a collision continue call
					else
					{
						//TODO
					}
				}

			}
			
			if (colliding == false)
			{
				delete collision;
				delete oppositeCollision;
			}
		}
	}

	//Any collisions that were active last frame that are not active now can be untracked
	for (unsigned int i = 0; i < lastFrameActiveCollisions.size(); i++)
	{
		Collision* toCheck = lastFrameActiveCollisions[i];
		bool activeThisFrame = false;
		for (unsigned int j = 0; j < activeCollisions.size(); j++)
		{
			if (toCheck == activeCollisions[j])
			{
				activeThisFrame = true;
				break;
			}
		}

		if (!activeThisFrame)
		{
			delete lastFrameActiveCollisions[i];
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
void PhysicsManager::AddSoftbody(Softbody* softbody)
{
	softbodies[softbody] = 0;
}
void PhysicsManager::AddCollider(Collider* collider)
{
	colliders[collider] = 0;
}

void PhysicsManager::RemoveRigidbody(Rigidbody* rigidbody)
{
	rigidbodies.erase(rigidbody);
}
void PhysicsManager::RemoveSoftbody(Softbody* softbody)
{
	softbodies.erase(softbody);
}
void PhysicsManager::RemoveCollider(Collider* collider)
{
	colliders.erase(collider);
}
