#ifndef QUATERNION_H
#define QUATERNION_H

//Pre-Include Defines
#define _USE_MATH_DEFINES

//DLL Header
#include "BrickwareMath/BrickwareMathDLL.hpp"

//System Level Headers
#include <iostream>
#include <cmath>

//Project Header
#include "BrickwareMath/Vector3.hpp"
#include "BrickwareMath/Vector4.hpp"
#include "BrickwareMath/Matrix3.hpp"

namespace Brickware
{
	namespace Math
	{
		class BRICKWARE_MATH_API Quaternion
		{
		public:
			Quaternion();
			Quaternion(float x, float y, float z, float w);
			Quaternion(Vector3 v);
			Quaternion(Vector4 v);

			~Quaternion();

			static Quaternion getQuaternionIdentity();

			Quaternion getInverse();
			Vector4 getAxisAngle();
			Matrix3 getRotationMatrix();

			//Accessors and mutators
			float getX();
			float getY();
			float getZ();
			float getW();

			void setX(float x);
			void setY(float y);
			void setZ(float z);
			void setW(float w);

			float& operator[] (int i);
			Quaternion operator* (Quaternion other);

			friend std::ostream& operator<< (std::ostream& output, Quaternion& h);
			friend std::istream& operator>> (std::istream& input, Quaternion& h);

		private:
			float q[4];
			void normalize();
		};
	}
}
#endif
