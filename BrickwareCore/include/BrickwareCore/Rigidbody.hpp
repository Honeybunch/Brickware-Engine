#ifndef RIGIDBODY_H
#define RIGIDBODY_H

//DLL Header
#include "BrickwareCoreDLL.h"

//Other Brickware Project Headers
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

//Project Headers
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API Rigidbody : public Component
		{
			friend class PhysicsManager;

		public:
			Rigidbody();

			//Accessors
			Math::Vector3 getVelocity();
			Math::Vector3 getSleepVelocity();

			Math::Vector3 getAngularVelocity();
			Math::Vector3 getAngluarSleepVelocity();

			Math::Vector3 getCenterOfMass();
			Math::Vector3 getWorldCenterOfMass();

			//Mutators
			void setMass(float mass);
			void setUseGravity(bool useGravity);
			void setIsKinematic(bool isKinematic);

			//Functions to manipulate rigidbody
			void addForce(Math::Vector3 force);
			void addForceAtPosition(Math::Vector3 force, Math::Vector3 position);
			void addExplosionForce(Math::Vector3 force, Math::Vector3 position, float radius);

			void addTorque(Math::Vector3 torque);

			void Start();
			void FixedUpdate();
			void OnCollision(Collision* collision);

			~Rigidbody();

		private:
			Math::Vector3 acceleration;
			Math::Vector3 velocity;
			Math::Vector3 sleepVelocity;

			Math::Vector3 angularAcceleration;
			Math::Vector3 angularVelocity;
			Math::Vector3 angularSleepVelocity;

			Math::Vector3 centerOfMass;
			Math::Vector3 worldCenterOfMass;

			Math::Vector3 inertiaTensor;

			Math::Matrix3 momentOfInertia();

			bool detectCollisions;
			bool isKinematic;
			bool useGravity;

			float mass;
			float drag;
			float angularDrag;
		};
	}
}
#endif
