#ifndef RAY_H
#define RAY_H

#include "BrickwareMathDLL.hpp"

#include "Vector3.hpp"

namespace Brickware
{
	namespace Math
	{
		class BRICKWARE_MATH_API Ray
		{
		public:
			Ray(Vector3 direction);
			Ray(Vector3 origin, Vector3 direction);

			Vector3 getOrigin();
			Vector3 getDirection();

			void setOrigin(Vector3 origin);
			void setDirection(Vector3 direction);

		private:
			Vector3 origin;
			Vector3 direction;
		};
	}
}

#endif