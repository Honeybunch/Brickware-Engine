#include "Rigidbody.h"


Rigidbody::Rigidbody()
{
	velocity = new Vector3();
	sleepVelocity = new Vector3(.1f, .1f, .1f);

	angularVelocity = new Vector3();
	sleepAngularVelocity = new Vector3(.1f, .1f, .1f);

	centerOfMass = new Vector3();
	worldCenterOfMass = new Vector3();

	mass = 1.0f;

	drag = 0.1f;
	angularDrag = 0.1f;

	detectCollisions = true;
	isKinematic = false;
	useGravity = true;
}

void Rigidbody::AddForce(Vector3 force)
{
	
}

void Rigidbody::AddTorque(Vector3 torque)
{

}

void Rigidbody::Update()
{
	
}

Rigidbody::~Rigidbody()
{
}
