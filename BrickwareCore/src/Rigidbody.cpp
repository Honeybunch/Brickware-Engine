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
	angularDrag = 1.0f;

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

void Rigidbody::addTorque(Vector3 torque)
{
	angularAcceleration += torque / mass;
}

//Called on a fixed timestep for physics calculations
void Rigidbody::FixedUpdate()
{
	//Derive velocity from acceleration 
	velocity += acceleration;
	angularVelocity += angularAcceleration;

	Transform* transform = getGameObject()->getTransform();

	//Apply velocity to position
	Vector3 position = transform->getPosition();
	position += velocity;
	transform->setPosition(position);

	//Apply angular velocity to rotation
	Vector3 eulerRotation = transform->getEulerRotation();
	eulerRotation += angularVelocity;
	transform->setRotation(eulerRotation);
	
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
	Vector3 normal = Vector3::Normalize(MTV);
	getGameObject()->getTransform()->setPosition(getGameObject()->getTransform()->getPosition() + MTV);

	//Calculate new forces
	float otherMass = 1.0f;
	Vector3 otherVelocity;
	Vector3 otherAngularVelocity;

	if (otherRigidbody)
	{
		otherMass = otherRigidbody->mass;
		otherVelocity = otherRigidbody->velocity;
		otherAngularVelocity = otherRigidbody->angularVelocity;
	}

	//Calculate which point we're going to use as the "Point of collision"
	std::vector<Vector3> pointsOfCollision = collision->getPointsOfCollision();
	Vector3 pointOfCollision;

	unsigned int bestPointIndex = 0;
	float bestPointDot = std::numeric_limits<float>::min();

	if (pointsOfCollision.size() > 1)
	{
		for (unsigned int i = 0; i < pointsOfCollision.size(); i++)
		{
			float dot = Vector3::Dot(pointsOfCollision[i], MTV);
			if (dot > bestPointDot)
			{
				bestPointDot = dot;
				bestPointIndex = i;
			}
		}
	}

	pointOfCollision = pointsOfCollision[bestPointIndex];

	//Determine the impulse based on Chris Hecker's formula
	float e = 0.5f; //elasticity
	Vector3 relativeVelocity = velocity - otherVelocity;
	float impulse = Vector3::Dot((relativeVelocity * mass) * -(1 + e), normal);
	impulse /= Vector3::Dot(normal, normal * ((1 / mass) + (1 / otherMass)));

	//Determine radius of collision point
	Vector3 radius = pointOfCollision - centerOfMass;
	Vector3 resultantAngularAccel = angularAcceleration + (Vector3::Cross(radius, normal * impulse));
	angularAcceleration = resultantAngularAccel;

	//Determine resulting velocity
	Vector3 finalVelocity = velocity + ((normal * impulse)/mass);
	velocity = finalVelocity;
}

Rigidbody::~Rigidbody()
{
	PhysicsManager::RemoveRigidbody(this);
}
