#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/Rigidbody.hpp"
#include "BrickwareCore/PhysicsManager.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;
using namespace Graphics;

Rigidbody::Rigidbody()
{
	
}

void Rigidbody::Start()
{
	transform = getGameObject()->getTransform();

	//We should probably get an inertia tensor from a collider or mesh but we'll just assume it's a box
	Vector3 scale = getGameObject()->getTransform()->getScale();
	float mass = body.getMass();

	float inertiaTensorX = (mass * (powf(scale[1] * 2, 2) + powf(scale[2] * 2, 2))) / 12;
	float inertiaTensorY = (mass * (powf(scale[0] * 2, 2) + powf(scale[2] * 2, 2))) / 12;
	float inertiaTensorZ = (mass * (powf(scale[0] * 2, 2) + powf(scale[1] * 2, 2))) / 12;

	body.setInertia(Vector3(inertiaTensorX, inertiaTensorY, inertiaTensorZ));

	//Register into the physics system
	PhysicsManager::AddRigidbody(this);
}

//Accessors
Vector3 Rigidbody::getVelocity(){ return body.getVelocity(); }

Vector3 Rigidbody::getAngularVelocity(){ return body.getAngularVelocity(); }

Vector3 Rigidbody::getCenterOfMass(){ return body.getCenterOfMass(); }

//Mutators
void Rigidbody::setMass(float mass){ body.setMass(mass); }
void Rigidbody::setIsKinematic(bool isKinematic){ this->isKinematic = isKinematic; }
void Rigidbody::setUseGravity(bool useGravity){ body.setUseGravity(useGravity); }

//Functions to manipulate rigidbody
void Rigidbody::addForce(Vector3 force, Vector3 point)
{
	body.addForce(force, point);
}

void Rigidbody::addImpulse(Vector3 impulse, Vector3 point)
{
	body.addImpulse(impulse, point);
}

void Rigidbody::addTorque(Vector3 torque)
{
	body.addTorque(torque);
}

void Rigidbody::addInstantaneousTorque(Vector3 instantTorque)
{
	body.addInstantaneousTorque(instantTorque);
}

//Called on a fixed timestep for physics calculations
void Rigidbody::FixedUpdate()
{
	float deltaTime = GameTime::GetFixedDeltaTime() * GameTime::GetTimeScale();

	Vector3 pos = transform->getPosition();
	Vector3 rot = transform->getEulerRotation();

	body.updateForces(deltaTime, &pos, &rot);

	transform->setPosition(pos);
	transform->setEulerRotation(rot);
}
void Rigidbody::OnCollision(Collision* collision)
{
	if (!isKinematic)
		return;
#ifdef _DEBUG
	if (Debug::Debugging)
		GameTime::SetTimeScale(0);
#endif

	Rigidbody* otherRigidbody = collision->getOtherRigidbody();
	Collider* otherCollider = collision->getOtherCollider();

	if (otherRigidbody == nullptr)
		return;

	Vector3 MTV = collision->getMTV();
	Vector3 normal = Vector3::Normalize(MTV);

	//Reposition rigidbody's game object back to where the collision happened so that it no longer intersects
	transform->setPosition(transform->getPosition() + MTV);

	//Calculate new forces
	Body otherBody = otherRigidbody->body;
	bool    otherIsKinematic = otherRigidbody->isKinematic;
	float   otherMass = otherBody.getMass();
	Vector3 otherVelocity = otherBody.getVelocity();
	Vector3 otherAngularVelocity = otherBody.getAngularVelocity();
	Vector3 otherCenterOfMass = otherBody.getCenterOfMass();
	Matrix3 otherRotationMatrix = otherRigidbody->getGameObject()->getTransform()->getRotation().getRotationMatrix();
	
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
			if (fabsf(dot) > fabsf(bestPointDot))
			{
				bestPointDot = dot;
				bestPointIndex = i;
			}
		}
	}

	pointOfCollision = pointsOfCollision[bestPointIndex];

	//Get radii
	Vector3 radius = pointOfCollision - (body.getCenterOfMass()) - MTV;
	Vector3 otherRadius = pointOfCollision - (otherCollider->getGameObject()->getTransform()->getPosition() + otherCenterOfMass) + MTV;	

	//Determine the impulse based on Chris Hecker's formula
	float e = 0.5f;
	Vector3 relativeVelocity;

	//Calculate the numerator of the impulse calculation
	Vector3 totalVelocity1 = Vector3::Cross(body.getVelocity(), radius);
	totalVelocity1 += body.getVelocity();

	Vector3 totalVelocity2;

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

	Vector3 velFromTorque1 = body.getInverseInertia() * torque1;
	velFromTorque1 = Vector3::Cross(velFromTorque1, radius);

	Vector3 velFromTorque2;
	if (otherIsKinematic)
	{
		velFromTorque2 = otherBody.getInverseInertia() * torque2;
		velFromTorque2 = Vector3::Cross(velFromTorque2, otherRadius);
	}

	Vector3 velFromTorques = velFromTorque1;
	velFromTorques += velFromTorque2;

	float inverseMassSum = body.getInverseMass();
	if (otherIsKinematic)
		inverseMassSum += otherBody.getInverseMass();

	float denominator = inverseMassSum + Vector3::Dot(velFromTorques, MTV);

	//Finally calculate impulse
	float impulse = numerator / denominator;
	Vector3 impulseVec = normal * impulse;
 	addImpulse(impulseVec, radius);

	//Immediately integrate
	//FixedUpdate();
}

Rigidbody::~Rigidbody()
{
	PhysicsManager::RemoveRigidbody(this);
}
