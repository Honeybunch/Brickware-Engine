#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

//DLL Header
#include "BrickwareCoreDLL.h"

//Project Headers
#include "Collider.h"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API SphereCollider : public Collider
		{
		public:
			SphereCollider(Math::Vector3 center = Math::Vector3(0, 0, 0), float radius = 1.0f);

			Math::Vector3 getCenter();
			float getRadius();

			~SphereCollider();

		private:
			Math::Vector3 center; //Measured in local space
			float radius;

			bool isCollidingWithSphere(SphereCollider* other);
			bool isCollidingWithBox(BoxCollider* other);
			bool isCollidingWithFrustum(FrustumCollider* other);
			bool isCollidingWithBounds(Math::Bounds* other);
		};
	}
}
#endif