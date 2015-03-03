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

	drag = 0.1f;
	angularDrag = 0.1f;

	detectCollisions = true;
	isKinematic = false;
	useGravity = true;

	//Apply gravity
	if (useGravity)
		acceleration = Vector3(0, PhysicsManager::gravity, 0);
}

void Rigidbody::Start()
{
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
	if (velocity[1] < sleepVelocity[1] && acceleration[1] < 0)
		velocity[1] = 0;
	if (velocity[1] > -sleepVelocity[1] && acceleration[1] > 0)
		velocity[1] = 0;

}

Rigidbody::~Rigidbody()
{
	PhysicsManager::RemoveRigidbody(this);
}
