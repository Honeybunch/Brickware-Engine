#ifndef QUATERNION_H
#define QUATERNION_H

//Pre-Include Defines
#define _USE_MATH_DEFINES

//DLL Header
#include "BrickwareMathDLL.h"

//System Level Headers
#include <iostream>
#include <math.h>

//Project Header
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"

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