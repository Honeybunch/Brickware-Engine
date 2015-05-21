#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/Rigidbody.hpp"
#include "BrickwareCore/PhysicsManager.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;
using namespace Graphics;

Rigidbody::Rigidbody()
{
	body = new Body();
}

void Rigidbody::Start()
{
	transform = getGameObject()->getTransform();

	//We should probably get an inertia tensor from a collider or mesh but we'll just assume it's a box
	Vector3 scale = getGameObject()->getTransform()->getScale();
	float mass = body->getMass();

	float inertiaTensorX = (mass * (powf(scale[1] * 2, 2) + powf(scale[2] * 2, 2))) / 12;
	float inertiaTensorY = (mass * (powf(scale[0] * 2, 2) + powf(scale[2] * 2, 2))) / 12;
	float inertiaTensorZ = (mass * (powf(scale[0] * 2, 2) + powf(scale[1] * 2, 2))) / 12;

	body->setInertia(Vector3(inertiaTensorX, inertiaTensorY, inertiaTensorZ));

	//Register into the physics system
	PhysicsManager::AddRigidbody(this);
}

//Accessors
Vector3 Rigidbody::getVelocity(){ return body->getVelocity(); }

Vector3 Rigidbody::getAngularVelocity(){ return body->getAngularVelocity(); }

Vector3 Rigidbody::getCenterOfMass(){ return body->getCenterOfMass(); }

//Mutators
void Rigidbody::setMass(float mass){ body->setMass(mass); }
void Rigidbody::setIsKinematic(bool isKinematic){ this->isKinematic = isKinematic; }
void Rigidbody::setUseGravity(bool useGravity){ body->setUseGravity(useGravity); }

//Functions to manipulate rigidbody
void Rigidbody::addForce(Vector3 force, Vector3 point)
{
	body->addForce(force, point);
}

void Rigidbody::addImpulse(Vector3 impulse, Vector3 point)
{
	body->addImpulse(impulse, point);
}

void Rigidbody::addTorque(Vector3 torque)
{
	body->addTorque(torque);
}

void Rigidbody::addInstantaneousTorque(Vector3 instantTorque)
{
	body->addInstantaneousTorque(instantTorque);
}

//Called on a fixed timestep for physics calculations
void Rigidbody::FixedUpdate()
{
	float deltaTime = GameTime::GetFixedDeltaTime() * GameTime::GetTimeScale();

	Vector3 pos = transform->getPosition();
	Vector3 rot = transform->getEulerRotation();

	body->updateForces(deltaTime, &pos, &rot);

	transform->setPosition(pos);
	transform->setEulerRotation(rot);
}
void Rigidbody::OnCollision(Collision* collision)
{

}

Rigidbody::~Rigidbody()
{
	PhysicsManager::RemoveRigidbody(this);
}
