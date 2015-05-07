#ifndef RIGIDBODY_H
#define RIGIDBODY_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareMath/Vector3.hpp"
#include "BrickwareMath/Vector4.hpp"
#include "BrickwareMath/Quaternion.hpp"

//Project Headers
#include "BrickwareCore/GameObject.hpp"
#include "BrickwareCore/Transform.hpp"
#include "BrickwareCore/Component.hpp"

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

			//Add force to system
			void addForce(Math::Vector3 force, Math::Vector3 point = Math::Vector3());
			
			//Add impulse to the system
			void addImpulse(Math::Vector3 impulse, Math::Vector3 point = Math::Vector3());

			//Adds torque to the system
			void addTorque(Math::Vector3 torque);
			void addInstantaneousTorque(Math::Vector3 instantTorque);

			//void addForceAtPosition(Math::Vector3 force, Math::Vector3 position);
			//void addExplosionForce(Math::Vector3 force, Math::Vector3 position, float radius);

			void Start();
			void FixedUpdate();
			void OnCollision(Collision* collision);

			~Rigidbody();

		private:
			Transform* transform;

			Math::Vector3 acceleration;
			Math::Vector3 velocity;
			Math::Vector3 sleepVelocity;

			Math::Vector3 angularAcceleration;
			Math::Vector3 angularVelocity;
			Math::Vector3 angularSleepVelocity;

			Math::Vector3 centerOfMass;
			Math::Vector3 worldCenterOfMass;

			Math::Vector3 inertiaTensor;
			Math::Matrix3 inertia;
			Math::Matrix3 inverseInertia;

			int collisionIterations;

			bool detectCollisions;
			bool isKinematic;
			bool useGravity;

			float mass;
			float inverseMass;
			float drag;
			float angularDrag;

			Math::Vector3 impulse;
			Math::Vector3 frameForce;
			Math::Vector3 frameTorque;
			Math::Vector3 frameInstantTorque;
		};
	}
}
#endif
