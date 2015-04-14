#ifndef MATRIX3_H
#define MATRIX3_H

//DLL Header
#include "BrickwareMathDLL.h"

//Project Headers
#include "Vector3.h"

namespace Brickware
{
	namespace Math
	{
		class Matrix4;

		class BRICKWARE_MATH_API Matrix3
		{
		public:
			Matrix3();
			Matrix3(float rawArray[]);
			Matrix3(float x0, float y0, float z0,
				float x1, float y1, float z1,
				float x2, float y2, float z2);
			Matrix3(Vector3 one, Vector3 two, Vector3 three);

			//Accessors
			Matrix3 getTranspose();
			Matrix3 getInverse();
			float* getAsArray();

			//Operators
			Matrix3 operator* (Matrix3 mat);
			Vector3 operator* (Vector3 vec);
			operator Matrix4();

			float* operator[] (int i);

			~Matrix3();

		private:
			float rawMatrix[9];
			float matrix[3][3];
		};
	}
}
#endif