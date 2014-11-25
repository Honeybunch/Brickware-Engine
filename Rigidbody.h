#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Component.h"
#include "Vector3.h"

class Rigidbody : public Component
{
public:
	Rigidbody();

	//Accessors

	//Mutators

	//Functions to manipulate rigidbody
	void AddForce(Vector3 force);
	void AddTorque(Vector3 torque);

	virtual void Update();

	~Rigidbody();

private:
	Vector3* velocity;
	Vector3* sleepVelocity;
	
	Vector3* angularVelocity;
	Vector3* sleepAngularVelocity;

	Vector3* centerOfMass;
	Vector3* worldCenterOfMass;

	bool detectCollisions;
	bool isKinematic;
	bool useGravity;

	float mass;
	float drag;
	float angularDrag;
};

#endif
