#ifndef MATRIX4_H
#define MATRIX4_H

#define _USE_MATH_DEFINES

//DLL Header
#include "BrickwareMath/BrickwareMathDLL.hpp"

//Project Headers
#include "BrickwareMath/Vector3.hpp"
#include "BrickwareMath/Vector4.hpp"

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
			
			static Matrix4 getOrthographicProjection(float left, float right, float bottom, float top, float near, float far);
			static Matrix4 getPerspectiveProjection(float fov, float aspect, float near, float far);

			static Matrix4 getLookAtView(Vector3 eye, Vector3 center, Vector3 up);

			//Accessors
			Matrix4 getTranspose();
			Matrix4 getInverse();
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
