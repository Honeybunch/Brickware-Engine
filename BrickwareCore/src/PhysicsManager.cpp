#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/PhysicsManager.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;
using namespace Graphics;

//Statics
std::vector<Collision*> PhysicsManager::activeCollisions;
std::vector<Collision*> PhysicsManager::lastFrameActiveCollisions;
std::unordered_map<Rigidbody*, int> PhysicsManager::rigidbodies;
std::unordered_map<Softbody*, int> PhysicsManager::softbodies;
std::unordered_map<Collider*, int> PhysicsManager::colliders;

Vector3 PhysicsManager::gravity;

Vector3 PhysicsManager::GetGravity(){ return gravity; }
void PhysicsManager::SetGravity(Vector3 gravity){ PhysicsManager::gravity = gravity; }

void PhysicsManager::Initialize()
{
	gravity[1] = -9.81f;
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
		Collider* testCollider = it1->first;
		if (testCollider->getGameObject()->getComponent<Rigidbody>() == nullptr)
			continue;

		for (auto it2 = colliders.begin(); it2 != colliders.end(); it2++)
		{
			Collider* otherCollider = it2->first;
			if (otherCollider->getGameObject()->getComponent<Rigidbody>() == nullptr)
				continue;

			bool colliding = false;
			Collision* collision = new Collision();
			Collision* oppositeCollision = new Collision();
			if (otherCollider != testCollider)
			{
				colliding = testCollider->isColliding(otherCollider, collision);
				otherCollider->isColliding(testCollider, oppositeCollision);
			}

			if (otherCollider != testCollider && colliding)
			{
				//Dispatch collision
				if (collision)
				{
					GameObject* testObj = testCollider->getGameObject();
					GameObject* otherObj = otherCollider->getGameObject();

					//If there isn't any identical collision, we will send a collision enter call
					if (IsCollisionActive(collision) == false && IsCollisionActive(oppositeCollision) == false)
					{
						testObj->OnCollisionEnter(collision);
						otherObj->OnCollisionEnter(oppositeCollision);

						activeCollisions.push_back(collision);
						activeCollisions.push_back(oppositeCollision);

#ifdef _DEBUG
						if (Debug::Debugging)
							GameTime::SetTimeScale(0);
#endif

						Vector3 MTV = collision->getMTV();
						Vector3 normal = Vector3::Normalize(MTV);

						//Reposition rigidbody's game objects back to where the collision happened so that it no longer intersects
						Transform* testTransform = testObj->getTransform();
						Transform* otherTransform = otherObj->getTransform();

						Rigidbody* testRigidbody = collision->getThisRigidbody();
						Rigidbody* otherRigidbody = collision->getOtherRigidbody();

						//Get some Variables
						Body*	testBody =				testRigidbody->body;
						bool    testIsKinematic =		testRigidbody->isKinematic;
						float   testMass =				testBody->getMass();
						Vector3 testVelocity =			testBody->getVelocity();
						Vector3 testAngularVelocity =	testBody->getAngularVelocity();
						Vector3 testCenterOfMass =		testBody->getCenterOfMass();
						Matrix3 testRotationMatrix =	testRigidbody->getGameObject()->getTransform()->getRotation().getRotationMatrix();

						Body*	otherBody =				otherRigidbody->body;
						bool    otherIsKinematic =		otherRigidbody->isKinematic;
						float   otherMass =				otherBody->getMass();
						Vector3 otherVelocity =			otherBody->getVelocity();
						Vector3 otherAngularVelocity =	otherBody->getAngularVelocity();
						Vector3 otherCenterOfMass =		otherBody->getCenterOfMass();
						Matrix3 otherRotationMatrix =	otherRigidbody->getGameObject()->getTransform()->getRotation().getRotationMatrix();

						if (otherRigidbody->isKinematic && !testRigidbody->isKinematic)
						{
							otherTransform->setPosition(otherTransform->getPosition() - MTV);
						}
						else if (testRigidbody->isKinematic && !otherRigidbody->isKinematic)
						{
							testTransform->setPosition(testTransform->getPosition() + MTV);
						}
						else if (testRigidbody->isKinematic && otherRigidbody->isKinematic)
						{
							testTransform->setPosition(testTransform->getPosition() + (MTV/2));
							otherTransform->setPosition(otherTransform->getPosition() - (MTV/2));
						}
						else
						{
							continue;
						}

						//Calculate which point we're going to use as the "Point of collision"
						std::vector<Vector3> pointsOfCollision = collision->getPointsOfCollision();
						Vector3 pointOfCollision;

						if (pointsOfCollision.size() == 0)
							return;

						unsigned int bestPointIndex = 0;
						float bestPointDot = 0;

						if (pointsOfCollision.size() > 1)
						{
							for (unsigned int i = 0; i < pointsOfCollision.size(); i++)
							{
								float dot = Vector3::Dot(pointsOfCollision[i], MTV);
								if (fabsf(dot) < fabsf(bestPointDot))
								{
									bestPointDot = dot;
									bestPointIndex = i;
								}
							}
						}

						pointOfCollision = pointsOfCollision[bestPointIndex];

						//Get radii
						Vector3 radius = pointOfCollision - (testTransform->getPosition() + testCenterOfMass) - MTV;
						Vector3 otherRadius = pointOfCollision - (otherTransform->getPosition() + otherCenterOfMass) + MTV;

						//Determine the impulse based on Chris Hecker's formula
						float e = 0.5f;
						Vector3 relativeVelocity;

						//Calculate the numerator of the impulse calculation
						Vector3 totalVelocity1;
						Vector3 totalVelocity2;
						
						if (testIsKinematic)
						{
							totalVelocity1 = Vector3::Cross(testAngularVelocity, radius);
							totalVelocity1 += testVelocity;
						}

						if (otherIsKinematic)
						{
							totalVelocity2 = Vector3::Cross(otherAngularVelocity, otherRadius);
							totalVelocity2 += otherVelocity;
						}

						relativeVelocity = totalVelocity1 - totalVelocity2;

						float relativeNormalVelocity = Vector3::Dot(relativeVelocity, normal);
						float numerator = (-1 - e) * relativeNormalVelocity;

						//Calculate the denominator
						Vector3 torque1 = Vector3::Cross(radius, MTV);
						Vector3 torque2 = Vector3::Cross(otherRadius, MTV);

						Vector3 velFromTorque1;
						Vector3 velFromTorque2;

						if (testIsKinematic)
						{
							velFromTorque1 = testBody->getInverseInertia() * torque1;
							velFromTorque1 = Vector3::Cross(velFromTorque1, radius);
						}
						
						if (otherIsKinematic)
						{
							velFromTorque2 = otherBody->getInverseInertia() * torque2;
							velFromTorque2 = Vector3::Cross(velFromTorque2, otherRadius);
						}

						Vector3 velFromTorques = velFromTorque1;
						velFromTorques += velFromTorque2;

						float inverseMassSum = 0;
						if (testIsKinematic)
							inverseMassSum = testBody->getInverseMass();
						if (otherIsKinematic)
							inverseMassSum += otherBody->getInverseMass();

						float denominator = inverseMassSum + Vector3::Dot(velFromTorques, MTV);

						//Finally calculate impulse
						float impulse = numerator / denominator;
						Vector3 impulseVec1 = normal * impulse;
						Vector3 impulseVec2 = (normal * -1) * impulse;
						
						if (testIsKinematic)
							testBody->addImpulse(impulseVec1, radius);
						if (otherIsKinematic)
							otherBody->addImpulse(impulseVec2 , otherRadius);
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

	//MESSY
	//Detect rigidbodies colliding with softbody nodes
	for (auto it1 = colliders.begin(); it1 != colliders.end(); it1++)
	{
		Collider* test = it1->first;
		if (test->getGameObject()->getComponent<Rigidbody>() == nullptr)
			continue;

		for (auto it2 = softbodies.begin(); it2 != softbodies.end(); it2++)
		{
			Softbody* softbody = it2->first;
			std::vector<Node*> nodes = softbody->nodes;

			for (unsigned int i = 0; i < nodes.size(); i++)
			{
				Node* node = nodes[i];
				Body* body = node->getBody();
				if (test->isColliding(softbody->getGameObject()->getTransform()->getPosition() + node->getPosition()))
				{
					//Apply impulses
					if (!node->getPinned())
						body->addForce(Vector3(0.0f, 0.0f, -10.0f));
				}
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
