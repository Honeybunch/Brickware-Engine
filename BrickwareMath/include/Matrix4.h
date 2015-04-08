#ifndef MATRIX4_H
#define MATRIX4_H

//DLL Header
#include "BrickwareMathDLL.h"

//Project Headers
#include "Vector3.h"
#include "Vector4.h"

namespace Brickware
{
	namespace Math
	{
		class Matrix3;

		class BRICKWARE_MATH_API Matrix4
		{
		public:
			Matrix4();
			Matrix4(float rawArray[]);
			Matrix4(float x0, float y0, float z0, float w0,
				float x1, float y1, float z1, float w1,
				float x2, float y2, float z2, float w2,
				float x3, float y3, float z3, float w3);
			Matrix4(Vector3 one, Vector3 two, Vector3 three, Vector3 four);
			Matrix4(Vector4 one, Vector4 two, Vector4 three, Vector4 four);

			static Matrix4 getIdentityMatrix();

			//Accessors
			float* getAsArray();

			//Operators
			float* operator[] (int i);
			Matrix4 operator* (Matrix4 mat);
			Vector3 operator* (Vector3 vec);
			Vector4 operator* (Vector4 vec);

			operator Matrix3();

			~Matrix4();

		private:
			float rawMatrix[16];
			float matrix[4][4];
		};
	}
}
#endif