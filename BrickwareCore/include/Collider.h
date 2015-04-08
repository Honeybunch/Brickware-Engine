#ifndef COLLIDER_H
#define COLLIDER_H

//DLL Headers
#include "BrickwareCoreDLL.h"

//Other Brickware Project Headers
#include "Matrix3.h"
#include "Quaternion.h"
#include "Vector3.h"
#include "Bounds.h"
#include "Primitive.h"

//System Level Headers
#include <vector>
#include <limits>

//Project Headers
#include "Component.h"

namespace Brickware
{
	namespace Core
	{
		class SphereCollider;
		class BoxCollider;
		class FrustumCollider;

		//Used to avoid warnings about exporting std::vectors
		template class BRICKWARE_CORE_API std::vector < Math::Vector3 > ;

		class BRICKWARE_CORE_API Collider : public Component
		{
		public:
			Collider();

			bool isColliding(Collider* collider, Collision* collsion);
			bool isColliding(Math::Bounds bounds);

			~Collider();

		protected:
			bool enabled;
			bool isTrigger;

			Math::Vector3 center;
			std::vector<Math::Vector3> normals;
			std::vector<Math::Vector3> worldNormals;

			void calculateWorldData();

		private:
			virtual bool isCollidingWithSphere(SphereCollider* other) = 0;
			virtual bool isCollidingWithBox(BoxCollider* other, Collision* collision) = 0;
			virtual bool isCollidingWithFrustum(FrustumCollider* other) = 0;
			virtual bool isCollidingWithBounds(Math::Bounds other) = 0;
		};
	}
}

#endif