#ifndef VECTOR4_H
#define VECTOR4_H

//DLL Header
#include "BrickwareMath/BrickwareMathDLL.hpp"

//System Level Headers
#include <iostream>
#include <cmath>

namespace Brickware
{
	namespace Math
	{
		class Vector3;

		class BRICKWARE_MATH_API Vector4
		{
		public:
			//Constructors
			Vector4();
			Vector4(float x, float y, float z, float w);
			Vector4(Vector3 v, float w);

			//Destructor
			~Vector4();

			//Accessors and mutators
			float getX();
			float getY();
			float getZ();
			float getW();

			float* getAsArray();

			void setX(float x);
			void setY(float y);
			void setZ(float z);
			void setW(float w);

			float& operator[] (int i);

			friend std::ostream& operator<< (std::ostream& output, Vector4& v4);
			friend std::istream& operator>> (std::istream& input, Vector4& v4);

			operator Vector3();

		private:
			float vector[4];
		};
	}
}
#endif
