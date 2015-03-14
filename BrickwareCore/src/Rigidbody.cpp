#define BRICKWARE_CORE_EXPORTS

#include "Rigidbody.h"
#include "PhysicsManager.h"

Rigidbody::Rigidbody()
{
	velocity = Vector3();
	sleepVelocity = Vector3(.1f, .1f, .1f);

	angularVelocity = Vector3();
	angularSleepVelocity = Vector3(.1f, .1f, .1f);

	centerOfMass = Vector3();
	worldCenterOfMass = Vector3();

	mass = 1.0f;

	drag = .90f;
	angularDrag = .90f;

	detectCollisions = true;
	isKinematic = false;
	useGravity = true;
}

void Rigidbody::Start()
{
	//Apply gravity
	if (useGravity)
		acceleration = Vector3(0, PhysicsManager::gravity, 0);

	//Register into the physics system
	PhysicsManager::AddRigidbody(this);
}

//Accessors
Vector3 Rigidbody::getVelocity(){ return velocity; }
Vector3 Rigidbody::getSleepVelocity(){ return sleepVelocity; }

Vector3 Rigidbody::getAngularVelocity(){ return angularVelocity; }
Vector3 Rigidbody::getAngluarSleepVelocity(){ return angularSleepVelocity; }

Vector3 Rigidbody::getCenterOfMass(){ return centerOfMass; }
Vector3 Rigidbody::getWorldCenterOfMass(){ return worldCenterOfMass; }

//Mutators
void Rigidbody::setUseGravity(bool useGravity){ this->useGravity = useGravity; }

//Functions to manipulate rigidbody
void Rigidbody::addForce(Vector3 force)
{
	//Derive acceleration from the applied force divided by the object's mass
	acceleration += force / mass;
}
void Rigidbody::addForceAtPosition(Vector3 force, Vector3 position){}
void Rigidbody::addExplosionForce(Vector3 force, Vector3 position, float radius){}

void Rigidbody::addTorque(Vector3 torque){}

//Called on a fixed timestep for physics calculations
void Rigidbody::FixedUpdate()
{
	//Derive velocity from acceleration 
	velocity += acceleration;

	//Apply velocity to position
	Vector3 position = getGameObject()->getTransform()->getPosition();
	position += velocity;
	getGameObject()->getTransform()->setPosition(position);

	//If velocity is within range of the sleep velocity, we set the velocity to 0
	//if (velocity[1] < sleepVelocity[1] && acceleration[1] < 0)
	//	velocity[1] = 0;
	//if (velocity[1] > -sleepVelocity[1] && acceleration[1] > 0)
	//	velocity[1] = 0;

	//Apply drag
	acceleration *= drag;
	angularAcceleration *= angularDrag;

	//Add gravity
	if (useGravity)
		acceleration[1] += PhysicsManager::gravity;
}
void Rigidbody::OnCollision(Collision* collision)
{
	Rigidbody* otherRigidbody = collision->getRigidbody();

	//Reposition rigidbody's game object back to where the collision happened so that it no longer intersects
	Vector3 MTV = collision->getMTV();
	getGameObject()->getTransform()->setPosition(getGameObject()->getTransform()->getPosition() + MTV);

	//Calculate new forces
	float otherMass = 1.0f;
	Vector3 otherVelocity;

	if (otherRigidbody)
	{
		otherMass = otherRigidbody->mass;
		otherVelocity = otherRigidbody->velocity;
	}

	float massTotal = otherMass + mass;
	Vector3 finalVelocity;

	//Perform calculations on every axis
	for (unsigned int i = 0; i < 3; i++)
	{
		float velComp = velocity[i];
		float otherVelComp = otherVelocity[i];

		float finalVelComp = ((velComp * mass) - (otherVelComp * otherMass)) / massTotal;
		finalVelocity[i] = finalVelComp;
	}

	velocity = finalVelocity;
}

Rigidbody::~Rigidbody()
{
	PhysicsManager::RemoveRigidbody(this);
}
