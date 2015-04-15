#ifndef VECTOR2_H
#define VECTOR2_H

//DLL Header
#include "BrickwareMath\BrickwareMathDLL.hpp"

//System Level Headers
#include <iostream>

namespace Brickware
{
	namespace Math
	{
		class BRICKWARE_MATH_API Vector2
		{
		public:
			//Constructors
			Vector2();
			Vector2(float x, float y);
			Vector2(const Vector2& other);

			//Destructor
			~Vector2();

			//Accessors & Mutators
			float getX();
			float getY();

			float* getAsArray();

			void setX(float x);
			void setY(float y);

			//Static functions
			static float Dot(Vector2 v, Vector2 u);
			static Vector2 Normalize(Vector2 v);

			//Operators
			float operator* (Vector2 u);
			Vector2 operator* (float s);
			Vector2 operator+ (Vector2 u);
			float& operator[] (int i);

			friend std::ostream& operator<< (std::ostream& output, Vector2& h);
			friend std::istream& operator>> (std::istream& input, Vector2& h);

		private:
			float vector[2];
		};
	}
}
#endif