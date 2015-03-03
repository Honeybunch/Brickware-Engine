#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "BrickwareCoreDLL.h"

#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "Vector3.h"

class BRICKWARE_CORE_API Rigidbody : public Component
{
	friend class PhysicsManager;

public:
	Rigidbody();

	//Accessors
	Vector3 getVelocity();
	Vector3 getSleepVelocity();

	Vector3 getAngularVelocity();
	Vector3 getAngluarSleepVelocity();

	Vector3 getCenterOfMass();
	Vector3 getWorldCenterOfMass();

	//Mutators

	//Functions to manipulate rigidbody
	void addForce(Vector3 force);
	void addForceAtPosition(Vector3 force, Vector3 position);
	void addExplosionForce(Vector3 force, Vector3 position, float radius);

	void addTorque(Vector3 torque);

	void Start();
	void FixedUpdate();

	~Rigidbody();

private:
	Vector3 acceleration;
	Vector3 velocity;
	Vector3 sleepVelocity;
	
	Vector3 angularAcceleration;
	Vector3 angularVelocity;
	Vector3 angularSleepVelocity;

	Vector3 centerOfMass;
	Vector3 worldCenterOfMass;

	bool detectCollisions;
	bool isKinematic;
	bool useGravity;

	float mass;
	float drag;
	float angularDrag;
};

#endif
