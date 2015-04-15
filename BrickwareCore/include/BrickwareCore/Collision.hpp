#ifndef COLLISION_H
#define COLLISION_H

//DLL Header
#include "BrickwareCoreDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareMath\Vector3.hpp"

//System Level Headers
#include <vector>

namespace Brickware
{
	namespace Core
	{
		class Rigidbody;
		class Collider;
		class GameObject;

		template class BRICKWARE_CORE_API std::vector < Math::Vector3 > ;

		class BRICKWARE_CORE_API Collision
		{
			friend class PhysicsManager;

		public:
			GameObject* getThisObject();
			Rigidbody* getThisRigidbody();
			Collider* getThisCollider();
			GameObject* getOtherObject();
			Rigidbody* getOtherRigidbody();
			Collider* getOtherCollider();
			Math::Vector3 getMTV();
			std::vector<Math::Vector3> getPointsOfCollision();

			void setThisObject(GameObject* gameObject);
			void setThisRigidbody(Rigidbody* rigidbody);
			void setThisCollider(Collider* collider);
			void setOtherObject(GameObject* gameObject);
			void setOtherRigidbody(Rigidbody* rigidbody);
			void setOtherCollider(Collider* collider);
			void setMTV(Math::Vector3 MTV);
			void setPointsOfCollision(std::vector<Math::Vector3> pointsOfCollision);

			virtual ~Collision();

		private:
			Collision();

			GameObject* thisObject;
			Rigidbody* thisRigidbody;
			Collider* thisCollider;
			GameObject* otherObject;
			Rigidbody* otherRigidbody;
			Collider* otherCollider;
			Math::Vector3 MTV;
			std::vector<Math::Vector3> pointsOfCollision;
		};
	}
}
#endif