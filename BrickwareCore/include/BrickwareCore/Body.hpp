#ifndef BODY_H
#define BODY_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//Other Brickware project headers
#include "BrickwareMath/Vector3.hpp"
#include "BrickwareMath/Matrix3.hpp"
#include "BrickwareMath/Matrix4.hpp"
#include "BrickwareMath/Quaternion.hpp"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API Body
		{
		public:
			Body(float mass = 1.0f);
			virtual ~Body();

			//Accessors
			float getMass();
			float getInverseMass();

			Math::Matrix3 getInertia();
			Math::Matrix3 getInverseInertia();

			Math::Vector3 getVelocity();
			Math::Vector3 getAngularVelocity();
			Math::Vector3 getCenterOfMass();

			//Mutators
			void setMass(float mass);
			void setInertia(Math::Vector3 inertiaTensor);
			void setUseGravity(bool useGravity);

			//Add force to system
			void addForce(Math::Vector3 force, Math::Vector3 point = Math::Vector3());

			//Add impulse to the system
			void addImpulse(Math::Vector3 impulse, Math::Vector3 point = Math::Vector3());

			//Add torque to the system
			void addTorque(Math::Vector3 torque);

			//Add instant torque to the system
			void addInstantaneousTorque(Math::Vector3 instantTorque);

			//Integrates all forces this frame onto the body
			void updateForces(float deltaTime, Math::Vector3* newPos, Math::Quaternion* newRot);

		private:
			Math::Vector3 acceleration;
			Math::Vector3 velocity;

			Math::Vector3 angularAcceleration;
			Math::Vector3 angularVelocity;

			Math::Vector3 centerOfMass;

			Math::Vector3 inertiaTensor;
			Math::Matrix3 inertia;
			Math::Matrix3 inverseInertia;

			int collisionIterations;

			bool isKinematic;
			bool useGravity;

			float mass;
			float inverseMass;

			Math::Vector3 impulse;
			Math::Vector3 frameForce;
			Math::Vector3 frameTorque;
			Math::Vector3 frameInstantTorque;
		};
	}
}

#endif