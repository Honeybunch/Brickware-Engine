#define BRICKWARE_MATH_EXPORTS

#include "BrickwareMath/Matrix4.hpp"
#include "BrickwareMath/Matrix3.hpp"

using namespace Brickware;
using namespace Math;

/*
	Statics
*/

Matrix4 Matrix4::getIdentityMatrix()
{
	return Matrix4();
}

Matrix4 Matrix4::getOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
{
	float a = 2 / (right - left);
	float b = 2 / (top - bottom);
	float c = -2 / (far - near);
	float d = -1 * ((right + left) / (right - left));
	float e = -1 * ((top + bottom) / (top - bottom));
	float f = (far + near) / (far - near);

	return Matrix4(a, 0, 0, 0,
				   0, b, 0, 0,
				   0, 0, c, 0, 
				   d, e, f, 1);
}

Matrix4 Matrix4::getPerspectiveProjection(float fov, float width, float height, float near, float far)
{
	float depth = far - near;
	float q = -(far + near) / depth;
	float qn = -2 * (far * near) / depth;

	float w = 2 * near / width;
	w /= (width / height);
	float h = 2 * near / height;

	return Matrix4 (w, 0, 0, 0,
					0, h, 0, 0,
					0, 0, q, -1,
					0, 0, qn, 0);
}

Matrix4 Matrix4::getLookAtView(Vector3 eye, Vector3 center, Vector3 up)
{
	//Calculate axes 
	Vector3 zAxis = Vector3::Normalize((eye - center));
	Vector3 xAxis = Vector3::Normalize(Vector3::Cross(up, zAxis));
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

	//Create view matrix;
	return Matrix4(xAxis.getX(), yAxis.getX(), zAxis.getX(), 0,
				   xAxis.getY(), yAxis.getY(), zAxis.getY(), 0,
				   xAxis.getZ(), yAxis.getZ(), zAxis.getZ(), 0,
				   Vector3::Dot(xAxis * -1, eye),
				   Vector3::Dot(yAxis * -1, eye),
				   Vector3::Dot(zAxis * -1, eye),
				   1);
}

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

	//Inversion using Cramer's Rule adapted from https://graphics.stanford.edu/~mdfisher/Code/Engine/Matrix4.cpp.html

	float tmp[12]; //Temp array for pairs
	float src[16]; //Transpose of source matrix
	float determinant; 

	//Transpose into float
	for (unsigned int i = 0; i < 4; i++)
	{
		src[i + 0] = matrix[i][0];
		src[i + 4] = matrix[i][1];
		src[i + 8] = matrix[i][2];
		src[i + 12] = matrix[i][3];
	}

	//Calculate pairs for first 8 elements (cofactors)
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	//Calculate first 8 elements (cofactors)
	inverse[0][0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	inverse[0][0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	inverse[0][1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	inverse[0][1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	inverse[0][2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	inverse[0][2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	inverse[0][3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	inverse[0][3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	inverse[1][0] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	inverse[1][0] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	inverse[1][1] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	inverse[1][1] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	inverse[1][2] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	inverse[1][2] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	inverse[1][3] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	inverse[1][3] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
	//Calculate pairs for second 8 elements (cofactors)
	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];

	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];
	//Calculate second 8 elements (cofactors)
	inverse[2][0] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	inverse[2][0] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	inverse[2][1] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	inverse[2][1] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	inverse[2][2] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	inverse[2][2] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	inverse[2][3] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	inverse[2][3] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	inverse[3][0] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	inverse[3][0] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	inverse[3][1] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	inverse[3][1] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	inverse[3][2] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	inverse[3][2] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	inverse[3][3] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	inverse[3][3] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];
	
	//Calculate inverse determinant
	determinant = src[0] * inverse[0][0] + src[1] * inverse[0][1] + src[2] * inverse[0][2] + src[3] * inverse[0][3];
	determinant = 1.0f / determinant;

	//Multiply inverse determinant to all memebers
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			inverse[i][j] *= determinant;
		}
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
