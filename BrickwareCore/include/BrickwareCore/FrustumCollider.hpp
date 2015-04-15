#ifndef FRUSTRUMCOLLIDER_H
#define FRUSTRUMCOLLIDER_H

//Pre-Include Headers
#define USE_MATH_DEFINES

//DLL Header
#include "BrickwareCore\BrickwareCoreDLL.hpp"

//System Level Header
#include <math.h>

//Project Headers
#include "BrickwareCore\Collider.hpp"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API FrustumCollider : public Collider
		{
		public:
			FrustumCollider(float zNear, float zFar, float FoV, float aspectRatio);

			virtual void Update();
			virtual void Render();

			~FrustumCollider();

		private:
			float zNear;
			float zFar;

			float frustumNearWidth;
			float frustumNearHeight;

			float frustumFarWidth;
			float frustumFarHeight;

			Math::Vector3 nearBottomLeft;
			Math::Vector3 nearBottomRight;
			Math::Vector3 nearTopLeft;
			Math::Vector3 nearTopRight;

			Math::Vector3 farBottomLeft;
			Math::Vector3 farBottomRight;
			Math::Vector3 farTopLeft;
			Math::Vector3 farTopRight;

			bool isCollidingWithSphere(SphereCollider* other);
			bool isCollidingWithBox(BoxCollider* other);
			bool isCollidingWithFrustum(FrustumCollider* other);
			bool isCollidingWithBounds(Math::Bounds other);
		};
	}
}
#endif