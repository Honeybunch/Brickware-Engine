#define BRICKWARE_MATH_EXPORTS

#include "BrickwareMath\Matrix3.hpp"
#include "BrickwareMath\Matrix4.hpp"

using namespace Brickware;
using namespace Math;

/*
	Constructors
*/

Matrix3::Matrix3()
{
	matrix[0][0] = 1.0f; matrix[0][1] = 0.0f; matrix[0][2] = 0.0f;
	matrix[1][0] = 0.0f; matrix[1][1] = 1.0f; matrix[1][2] = 0.0f;
	matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = 1.0f;
}

Matrix3::Matrix3(float rawArray[])
{
	int index = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			matrix[i][j] = rawArray[index];
			index++;
		}
	}
}

Matrix3::Matrix3(float x0, float y0, float z0,
				 float x1, float y1, float z1,
				 float x2, float y2, float z2)
{
	matrix[0][0] = x0; matrix[0][1] = y0; matrix[0][2] = z0;
	matrix[1][0] = x1; matrix[1][1] = y1; matrix[1][2] = z1;
	matrix[2][0] = x2; matrix[2][1] = y2; matrix[2][2] = z2;
}

Matrix3::Matrix3(Vector3 one, Vector3 two, Vector3 three)
{
	matrix[0][0] = one.getX();	 matrix[0][1] = one.getY();	  matrix[0][2] = one.getZ();
	matrix[1][0] = two.getX();	 matrix[1][1] = two.getY();	  matrix[1][2] = two.getZ();
	matrix[2][0] = three.getX(); matrix[2][1] = three.getY(); matrix[2][2] = three.getZ();
}

/*
	Accessors and Mutators
*/

Matrix3 Matrix3::getTranspose()
{
	Matrix3 transpose;

	transpose[0][0] = matrix[0][0];
	transpose[1][1] = matrix[1][1];
	transpose[2][2] = matrix[2][2];

	transpose[0][1] = matrix[1][0];
	transpose[0][2] = matrix[2][0];
	transpose[1][2] = matrix[2][1];

	transpose[1][0] = matrix[0][1];
	transpose[2][0] = matrix[0][2];
	transpose[2][1] = matrix[1][2];

	return transpose;
}

Matrix3 Matrix3::getInverse()
{
	Matrix3 inverse;

	float determinant = (matrix[0][0] * ((matrix[1][1] * matrix[2][2]) - (matrix[2][1] * matrix[1][2])))
					  - (matrix[0][1] * ((matrix[1][0] * matrix[2][2]) - (matrix[1][2] * matrix[2][0])))
					  + (matrix[0][2] * ((matrix[1][0] * matrix[2][1]) - (matrix[1][1] * matrix[2][0])));

	//Return an identity matrix if there is no inverse
	if (determinant > 0)
	{
		float inverseDeterminant = 1 / determinant;

		inverse[0][0] =  ((matrix[1][1] * matrix[2][2]) - (matrix[2][1] * matrix[1][2])) * inverseDeterminant;
		inverse[0][1] =  ((matrix[0][2] * matrix[2][1]) - (matrix[0][1] * matrix[2][2])) * inverseDeterminant;
		inverse[0][2] =  ((matrix[0][1] * matrix[1][2]) - (matrix[0][2] * matrix[1][1])) * inverseDeterminant;
		inverse[1][0] =  ((matrix[1][2] * matrix[2][0]) - (matrix[1][0] * matrix[2][2])) * inverseDeterminant;
		inverse[1][1] =  ((matrix[0][0] * matrix[2][2]) - (matrix[0][2] * matrix[2][0])) * inverseDeterminant;
		inverse[1][2] =  ((matrix[1][0] * matrix[0][2]) - (matrix[0][0] * matrix[1][2])) * inverseDeterminant;
		inverse[2][0] =  ((matrix[1][0] * matrix[2][1]) - (matrix[2][0] * matrix[1][1])) * inverseDeterminant;
		inverse[2][1] =  ((matrix[2][0] * matrix[0][1]) - (matrix[0][0] * matrix[2][1])) * inverseDeterminant;
		inverse[2][2] =  ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1])) * inverseDeterminant;
	}

	return inverse;
}

float* Matrix3::getAsArray()
{
	int index = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
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

Matrix3 Matrix3::operator*(Matrix3 m)
{
	float product[9];
	int index = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			float value = (matrix[j][0] * m[0][i]) +
						  (matrix[j][1] * m[1][i]) +
						  (matrix[j][2] * m[2][i]);
			product[index] = value;

			index++;
		}
	}

	Matrix3 newMat(product);

	return newMat;
}

Vector3 Matrix3::operator*(Vector3 vec)
{
	float x, y, z;

	x = (matrix[0][0] * vec.getX()) + (matrix[1][0] * vec.getY()) + (matrix[2][0] * vec.getZ());
	y = (matrix[0][1] * vec.getX()) + (matrix[1][1] * vec.getY()) + (matrix[2][1] * vec.getZ());
	z = (matrix[0][2] * vec.getX()) + (matrix[1][2] * vec.getY()) + (matrix[2][2] * vec.getZ());

	return Vector3(x, y, z);
}

Matrix3::operator Matrix4()
{
	Matrix4 mat(matrix[0][0], matrix[0][1], matrix[0][2], 0,
				matrix[1][0], matrix[1][1], matrix[1][2], 0, 
				matrix[2][0], matrix[2][1], matrix[2][2], 0, 
				0, 0, 0, 1);

	return mat;
}

float* Matrix3::operator[](int i){ return matrix[i]; }

/*
	Destructor
*/

Matrix3::~Matrix3(){}