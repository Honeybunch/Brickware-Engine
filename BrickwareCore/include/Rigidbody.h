#ifndef RIGIDBODY_H
#define RIGIDBODY_H

//Only use this line to make sure VS creates an import lib
#ifdef BRICKWARE_CORE_EXPORTS
#define BRICKWARE_CORE_API __declspec(dllexport)
#else
#define BRICKWARE_CORE_API __declspec(dllimport)
#endif

#include "Component.h"
#include "Vector3.h"

class BRICKWARE_CORE_API Rigidbody : public Component
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
