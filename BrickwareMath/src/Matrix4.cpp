#define BRICKWARE_MATH_EXPORTS

#include "BrickwareMath\Matrix4.hpp"
#include "BrickwareMath\Matrix3.hpp"

using namespace Brickware;
using namespace Math;

/*
	Constructors
*/

Matrix4::Matrix4()
{
	matrix[0][0] = 1.0f; matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f; matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;
}

Matrix4::Matrix4(float rawArray[])
{
	int index = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix[i][j] = rawArray[index];
			index++;
		}
	}
}

Matrix4::Matrix4(float x0, float y0, float z0, float w0,
				 float x1, float y1, float z1, float w1,
				 float x2, float y2, float z2, float w2,
				 float x3, float y3, float z3, float w3)
{
	matrix[0][0] = x0; matrix[0][1] = y0; matrix[0][2] = z0; matrix[0][3] = w0;
	matrix[1][0] = x1; matrix[1][1] = y1; matrix[1][2] = z1; matrix[1][3] = w1;
	matrix[2][0] = x2; matrix[2][1] = y2; matrix[2][2] = z2; matrix[2][3] = w2;
	matrix[3][0] = x3; matrix[3][1] = y3; matrix[3][2] = z3; matrix[3][3] = w3;
}

Matrix4::Matrix4(Vector3 one,
				 Vector3 two,
				 Vector3 three,
				 Vector3 four)
{
	matrix[0][0] = one.getX();    matrix[0][1] = one.getY();    matrix[0][2] = one.getZ();   matrix[0][3] = 0;
	matrix[1][0] = two.getX();    matrix[1][1] = two.getY();    matrix[1][2] = two.getZ();   matrix[1][3] = 0;
	matrix[2][0] = three.getX();  matrix[2][1] = three.getY();  matrix[2][2] = three.getZ(); matrix[2][3] = 0;
	matrix[3][0] = four.getX();   matrix[3][1] = four.getY();   matrix[3][2] = four.getZ();  matrix[3][3] = 0;
}

Matrix4::Matrix4(Vector4 one,
				 Vector4 two,
				 Vector4 three,
				 Vector4 four)
{
	matrix[0][0] = one.getX();   matrix[0][1] = one.getY();   matrix[0][2] = one.getZ();   matrix[0][3] = one.getW();
	matrix[1][0] = two.getX();   matrix[1][1] = two.getY();   matrix[1][2] = two.getZ();   matrix[1][3] = two.getW();
	matrix[2][0] = three.getX(); matrix[2][1] = three.getY(); matrix[2][2] = three.getZ(); matrix[2][3] = three.getW();
	matrix[3][0] = four.getX();  matrix[3][1] = four.getY();  matrix[3][2] = four.getZ();  matrix[3][3] = four.getW();
}

/*
	Accessors and Mutators
*/

Matrix4 Matrix4::getTranspose()
{
	Matrix4 transpose;

	transpose[0][0] = matrix[0][0];
	transpose[1][1] = matrix[1][1];
	transpose[2][2] = matrix[2][2];
	transpose[3][3] = matrix[3][3];

	transpose[0][1] = matrix[1][0];
	transpose[0][2] = matrix[2][0];
	transpose[0][3] = matrix[3][0];
	transpose[1][2] = matrix[2][1];
	transpose[1][3] = matrix[3][1];
	transpose[2][3] = matrix[3][2];

	transpose[1][0] = matrix[0][1];
	transpose[2][0] = matrix[0][2];
	transpose[3][0] = matrix[0][3];
	transpose[2][1] = matrix[1][2];
	transpose[3][1] = matrix[1][3];
	transpose[3][2] = matrix[2][3];

	return transpose;
}

Matrix4 Matrix4::getInverse()
{
	Matrix4 inverse;

	//Adapted from Liam Middlebrook's implementation: https://github.com/liam-middlebrook/nautical/blob/master/include/math/matrix4.h

	float determinant =
		(matrix[0][0] * matrix[1][1] * matrix[2][2] * matrix[3][3]) + (matrix[0][0] * matrix[2][1] * matrix[3][2] * matrix[1][3]) +
		(matrix[0][0] * matrix[3][1] * matrix[1][2] * matrix[2][3]) + (matrix[1][0] * matrix[0][1] * matrix[3][2] * matrix[2][3]) +
		(matrix[1][0] * matrix[2][1] * matrix[0][2] * matrix[3][3]) + (matrix[1][0] * matrix[3][1] * matrix[2][2] * matrix[0][3]) +
		(matrix[2][0] * matrix[0][1] * matrix[1][2] * matrix[3][3]) + (matrix[2][0] * matrix[1][1] * matrix[3][2] * matrix[0][3]) +
		(matrix[2][0] * matrix[3][1] * matrix[0][2] * matrix[1][3]) + (matrix[3][0] * matrix[0][1] * matrix[2][2] * matrix[1][3]) +
		(matrix[3][0] * matrix[1][1] * matrix[0][2] * matrix[2][3]) + (matrix[3][0] * matrix[2][1] * matrix[1][2] * matrix[0][3]) -
		(matrix[0][0] * matrix[1][1] * matrix[3][2] * matrix[2][3]) - (matrix[0][0] * matrix[2][1] * matrix[1][2] * matrix[3][3]) -
		(matrix[0][0] * matrix[3][1] * matrix[2][2] * matrix[1][3]) - (matrix[1][0] * matrix[0][1] * matrix[2][2] * matrix[3][3]) -
		(matrix[1][0] * matrix[2][1] * matrix[3][2] * matrix[0][3]) - (matrix[1][0] * matrix[3][1] * matrix[0][2] * matrix[2][3]) -
		(matrix[2][0] * matrix[0][1] * matrix[3][2] * matrix[1][3]) - (matrix[2][0] * matrix[1][1] * matrix[0][2] * matrix[3][3]) -
		(matrix[2][0] * matrix[3][1] * matrix[1][2] * matrix[0][3]) - (matrix[3][0] * matrix[0][1] * matrix[1][2] * matrix[2][3]) -
		(matrix[3][0] * matrix[1][1] * matrix[2][2] * matrix[0][3]) - (matrix[3][0] * matrix[2][1] * matrix[0][2] * matrix[1][3]);

	//Return an identity matrix if there is no inverse
	if (determinant != 0)
	{
		float inverseDeterminant = 1 / determinant;

		//Row 1
		inverse[0][0] = ((matrix[1][1] * matrix[2][2] * matrix[3][3]) + (matrix[1][2] * matrix[2][3] * matrix[3][1]) + 
						 (matrix[1][3] * matrix[2][1] * matrix[3][2]) - (matrix[1][1] * matrix[2][3] * matrix[3][2]) -
						 (matrix[1][2] * matrix[2][1] * matrix[3][3]) - (matrix[1][3] * matrix[2][2] * matrix[3][1])) 
						 * inverseDeterminant;
		inverse[0][1] = ((matrix[0][1] * matrix[2][3] * matrix[3][2]) + (matrix[0][2] * matrix[2][1] * matrix[3][3]) +
						 (matrix[0][3] * matrix[2][2] * matrix[3][1]) - (matrix[0][1] * matrix[2][2] * matrix[3][3]) -
						 (matrix[0][2] * matrix[2][3] * matrix[3][1]) - (matrix[0][3] * matrix[2][1] * matrix[3][2]))
						 * inverseDeterminant;
		inverse[0][2] = ((matrix[0][1] * matrix[1][2] * matrix[3][3]) + (matrix[0][2] * matrix[1][3] * matrix[3][1]) +
						 (matrix[0][3] * matrix[1][1] * matrix[3][2]) - (matrix[0][1] * matrix[1][3] * matrix[3][2]) -
						 (matrix[0][2] * matrix[1][1] * matrix[3][3]) - (matrix[0][3] * matrix[1][2] * matrix[3][1]))
						 * inverseDeterminant;
		inverse[0][3] = ((matrix[0][1] * matrix[1][3] * matrix[2][2]) + (matrix[0][2] * matrix[1][1] * matrix[2][3]) +
						 (matrix[0][3] * matrix[1][2] * matrix[2][1]) - (matrix[0][1] * matrix[1][2] * matrix[2][3]) -
						 (matrix[0][2] * matrix[1][3] * matrix[2][1]) - (matrix[0][3] * matrix[1][1] * matrix[2][2]))
						 * inverseDeterminant;
				
		//Row 2
		inverse[1][0] = ((matrix[1][0] * matrix[2][3] * matrix[3][2]) + (matrix[1][2] * matrix[2][0] * matrix[3][3]) +
						 (matrix[1][3] * matrix[2][2] * matrix[3][0]) - (matrix[1][0] * matrix[2][2] * matrix[3][3]) -
						 (matrix[1][2] * matrix[2][3] * matrix[3][0]) - (matrix[1][3] * matrix[2][0] * matrix[3][2]))
						 * inverseDeterminant;
		inverse[1][1] = ((matrix[0][0] * matrix[2][2] * matrix[3][3]) + (matrix[0][2] * matrix[2][3] * matrix[3][0]) +
						 (matrix[0][3] * matrix[2][0] * matrix[3][2]) - (matrix[0][0] * matrix[2][3] * matrix[3][2]) -
						 (matrix[0][2] * matrix[2][0] * matrix[3][3]) - (matrix[0][3] * matrix[2][2] * matrix[3][0]))
						 * inverseDeterminant;
		inverse[1][2] = ((matrix[0][0] * matrix[1][3] * matrix[3][2]) + (matrix[0][2] * matrix[1][0] * matrix[3][3]) +
						 (matrix[0][3] * matrix[1][2] * matrix[3][0]) - (matrix[0][0] * matrix[1][2] * matrix[3][3]) -
						 (matrix[0][2] * matrix[1][3] * matrix[3][0]) - (matrix[0][3] * matrix[1][0] * matrix[3][2]))
						 * inverseDeterminant;
		inverse[1][3] = ((matrix[0][0] * matrix[1][2] * matrix[2][3]) + (matrix[0][2] * matrix[1][3] * matrix[2][0]) +
						 (matrix[0][3] * matrix[1][0] * matrix[2][2]) - (matrix[0][0] * matrix[1][3] * matrix[2][2]) -
						 (matrix[0][2] * matrix[1][0] * matrix[2][3]) - (matrix[0][3] * matrix[1][2] * matrix[2][0]))
						 * inverseDeterminant;

		//Row 3
		inverse[2][0] = ((matrix[1][0] * matrix[2][1] * matrix[3][3]) + (matrix[1][2] * matrix[2][0] * matrix[3][3]) +
						 (matrix[1][3] * matrix[2][0] * matrix[3][1]) - (matrix[1][0] * matrix[2][2] * matrix[3][3]) -
						 (matrix[1][1] * matrix[2][0] * matrix[3][3]) - (matrix[1][3] * matrix[2][0] * matrix[3][2]))
						 * inverseDeterminant;
		inverse[2][1] = ((matrix[0][0] * matrix[2][3] * matrix[3][1]) + (matrix[0][2] * matrix[2][3] * matrix[3][0]) +
						 (matrix[0][3] * matrix[2][1] * matrix[3][0]) - (matrix[0][0] * matrix[2][3] * matrix[3][2]) -
						 (matrix[0][1] * matrix[2][3] * matrix[3][0]) - (matrix[0][3] * matrix[2][2] * matrix[3][0]))
						 * inverseDeterminant;
		inverse[2][2] = ((matrix[0][0] * matrix[1][1] * matrix[3][3]) + (matrix[0][2] * matrix[1][0] * matrix[3][3]) +
						 (matrix[0][3] * matrix[1][0] * matrix[3][1]) - (matrix[0][0] * matrix[1][2] * matrix[3][3]) -
						 (matrix[0][1] * matrix[1][0] * matrix[3][3]) - (matrix[0][3] * matrix[1][0] * matrix[3][2]))
						 * inverseDeterminant;
		inverse[2][3] = ((matrix[0][0] * matrix[1][3] * matrix[2][1]) + (matrix[0][2] * matrix[1][3] * matrix[2][0]) +
						 (matrix[0][3] * matrix[1][1] * matrix[2][0]) - (matrix[0][0] * matrix[1][3] * matrix[2][2]) -
						 (matrix[0][1] * matrix[1][3] * matrix[2][0]) - (matrix[0][3] * matrix[1][2] * matrix[2][0]))
						 * inverseDeterminant;

		//Row 4
		inverse[3][0] = ((matrix[1][0] * matrix[2][2] * matrix[3][1]) + (matrix[1][1] * matrix[2][0] * matrix[3][2]) +
						 (matrix[1][2] * matrix[2][1] * matrix[3][0]) - (matrix[1][0] * matrix[2][1] * matrix[3][2]) -
						 (matrix[1][1] * matrix[2][2] * matrix[3][0]) - (matrix[1][2] * matrix[2][0] * matrix[3][1]))
						 * inverseDeterminant;
		inverse[3][1] = ((matrix[0][0] * matrix[2][1] * matrix[3][1]) + (matrix[0][1] * matrix[2][2] * matrix[3][0]) +
						 (matrix[0][2] * matrix[2][0] * matrix[3][2]) - (matrix[0][0] * matrix[2][2] * matrix[3][1]) -
						 (matrix[0][1] * matrix[2][0] * matrix[3][1]) - (matrix[0][2] * matrix[2][1] * matrix[3][0]))
						 * inverseDeterminant;
		inverse[3][2] = ((matrix[0][0] * matrix[1][2] * matrix[3][1]) + (matrix[0][1] * matrix[1][0] * matrix[3][2]) +
						 (matrix[0][2] * matrix[1][1] * matrix[3][0]) - (matrix[0][0] * matrix[1][1] * matrix[3][2]) -
						 (matrix[0][1] * matrix[1][2] * matrix[3][0]) - (matrix[0][2] * matrix[1][0] * matrix[3][1]))
						 * inverseDeterminant;
		inverse[3][3] = ((matrix[0][0] * matrix[1][1] * matrix[2][1]) + (matrix[0][1] * matrix[1][2] * matrix[2][0]) +
						 (matrix[0][2] * matrix[1][0] * matrix[2][2]) - (matrix[0][0] * matrix[1][2] * matrix[2][1]) -
						 (matrix[0][1] * matrix[1][0] * matrix[2][1]) - (matrix[0][2] * matrix[1][1] * matrix[2][0]))
						 * inverseDeterminant;
	}

	return inverse;
}

float* Matrix4::getAsArray() 
{
	int index = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			rawMatrix[index] = matrix[i][j];
			index++;
		}
	}
	return rawMatrix;
}

/*
	Operators
*/

float* Matrix4::operator[] (int i)
{
	return matrix[i];
}

Matrix4 Matrix4::operator*(Matrix4 m)
{
	float product[16];
	int index = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float value = (matrix[i][0] * m[0][j]) +
						  (matrix[i][1] * m[1][j]) +
						  (matrix[i][2] * m[2][j]) +
						  (matrix[i][3] * m[3][j]);
			product[index] = value;

			index++;
		}
	}

	Matrix4 newMat(product);

	return newMat;
}

Vector3 Matrix4::operator*(Vector3 vec)
{
	float x, y, z;

	x = (matrix[0][0] * vec.getX()) + (matrix[1][0] * vec.getY()) + (matrix[2][0] * vec.getZ()) + (matrix[3][0]);
	y = (matrix[0][1] * vec.getX()) + (matrix[1][1] * vec.getY()) + (matrix[2][1] * vec.getZ()) + (matrix[3][1]);
	z = (matrix[0][2] * vec.getX()) + (matrix[1][2] * vec.getY()) + (matrix[2][2] * vec.getZ()) + (matrix[3][2]);
	
	return Vector3(x, y, z);
}

Vector4 Matrix4::operator*(Vector4 vec)
{
	float x, y, z, w;

	x = (matrix[0][0] * vec.getX()) + (matrix[1][0] * vec.getY()) + (matrix[2][0] * vec.getZ()) + (matrix[3][0] * vec.getW());
	y = (matrix[0][1] * vec.getX()) + (matrix[1][1] * vec.getY()) + (matrix[2][1] * vec.getZ()) + (matrix[3][1] * vec.getW());
	z = (matrix[0][2] * vec.getX()) + (matrix[1][2] * vec.getY()) + (matrix[2][2] * vec.getZ()) + (matrix[3][2] * vec.getW());
	w = (matrix[0][3] * vec.getX()) + (matrix[1][3] * vec.getY()) + (matrix[2][3] * vec.getZ()) + (matrix[3][3] * vec.getW());

	return Vector4(x,y,z,w);
}

Matrix4::operator Matrix3()
{
	Matrix3 mat(matrix[0][0], matrix[0][1], matrix[0][2],
				matrix[1][0], matrix[1][1], matrix[1][2],
				matrix[2][0], matrix[2][1], matrix[2][2]);

	return mat;
}

/*
	Destructor
*/

Matrix4::~Matrix4(void)
{
	
}
