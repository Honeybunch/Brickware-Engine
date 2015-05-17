#ifndef RIGIDBODY_H
#define RIGIDBODY_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareMath/Vector3.hpp"
#include "BrickwareMath/Vector4.hpp"
#include "BrickwareMath/Quaternion.hpp"

#include "BrickwareGraphics/Primitive.hpp"

//Project Headers
#include "BrickwareCore/GameObject.hpp"
#include "BrickwareCore/Transform.hpp"
#include "BrickwareCore/Component.hpp"
#include "BrickwareCore/Body.hpp"

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
			Math::Vector3 getAngularVelocity();
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

			void Start();
			void FixedUpdate();
			void OnCollision(Collision* collision);

			~Rigidbody();

		private:
			Transform* transform;
			Body body;

			bool detectCollisions;
			bool isKinematic;
		};
	}
}
#endif
