#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/Body.hpp"
#include "BrickwareCore/PhysicsManager.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;

Body::Body(float mass)
{
	this->mass = mass;
	this->inverseMass = 1 / mass;

	centerOfMass = Vector3();

	setInertia(Vector3(1, 1, 1));

	useGravity = true;
}

Body::~Body()
{

}

//Accessors
float Body::getMass(){ return mass; }
float Body::getInverseMass(){ return inverseMass; }

Matrix3 Body::getInertia(){ return inertia; }
Matrix3 Body::getInverseInertia(){ return inverseInertia; }

Vector3 Body::getVelocity(){ return velocity; }
Vector3 Body::getAngularVelocity(){ return angularVelocity; }
Vector3 Body::getCenterOfMass(){ return centerOfMass; }

//Mutators
void Body::setMass(float mass)
{ 
	this->mass = mass;
	this->inverseMass = 1.0f / mass;
}

void Body::setInertia(Vector3 inertiaTensor)
{
	this->inertiaTensor = inertiaTensor;

	float inertiaTensorX = inertiaTensor[0];
	float inertiaTensorY = inertiaTensor[1];
	float inertiaTensorZ = inertiaTensor[2];

	float inverseInertiaTensorX = (1 / inertiaTensorX);
	float inverseInertiaTensorY = (1 / inertiaTensorY);
	float inverseInertiaTensorZ = (1 / inertiaTensorZ);

	inertia[0][0] = inertiaTensorX * mass;
	inertia[1][1] = inertiaTensorY * mass;
	inertia[2][2] = inertiaTensorZ * mass;

	inverseInertia[0][0] = inverseInertiaTensorX * inverseMass;
	inverseInertia[1][1] = inverseInertiaTensorY * inverseMass;
	inverseInertia[2][2] = inverseInertiaTensorZ * inverseMass;
}

void Body::setUseGravity(bool useGravity)
{ 
	this->useGravity = useGravity; 
}

//Add force to system
void Body::addForce(Vector3 force, Vector3 point)
{
	frameForce += force;

	Vector3 torque = Vector3::Cross(point, force);
	frameTorque += torque;
}

//Add impulse to the system
void Body::addImpulse(Vector3 impulse, Vector3 point)
{
	this->impulse += impulse;

	Vector3 instantTorque = Vector3::Cross(point, impulse);
	frameInstantTorque += instantTorque;
}

//Add torque to the system
void Body::addTorque(Vector3 torque)
{
	frameTorque += torque;
}

//Add instant torque to the system
void Body::addInstantaneousTorque(Vector3 instantTorque)
{
	frameInstantTorque += instantTorque;
}

//Integrates all forces this frame onto the body
void Body::updateForces(float deltaTime, Vector3* newPos, Vector3* newEulerRot)
{
	//Apply constant forces
	if (useGravity)
		acceleration = PhysicsManager::GetGravity() * inverseMass * deltaTime;
	else
		acceleration = Vector3();

	//Apply linear forces
	Vector3 VT, AT, VAT2;

	acceleration += frameForce * inverseMass;
	impulse *= inverseMass;

	VT = velocity * deltaTime;
	AT = acceleration * deltaTime;

	//AT^2
	VAT2 = AT * deltaTime;
	//AT^2 * 1/2
	VAT2 *= 0.5f;
	//VT + (AT^2 * 1/2)
	VAT2 += VT;

	(*newPos) += VAT2;

	velocity += AT;
	velocity += impulse;

	//Apply rotational forces
	Vector3 AAT, AVT;

	angularAcceleration = inverseInertia * frameTorque;
	frameInstantTorque = inverseInertia * frameInstantTorque;

	AAT = angularAcceleration * deltaTime;

	angularVelocity += AAT;
	angularVelocity += frameInstantTorque;

	AVT = angularVelocity * deltaTime;

	//Integrate angular velocity into rotation
	(*newEulerRot) += AVT;

	//Clear out frame forces
	impulse = Vector3();
	frameInstantTorque = Vector3();

	frameForce = Vector3();
	frameTorque = Vector3();
}