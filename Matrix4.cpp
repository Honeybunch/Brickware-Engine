#include "Matrix4.h"

Matrix4::Matrix4()
{
	matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f; matrix[3] = 0.0f;
	matrix[4] = 0.0f; matrix[5] = 1.0f; matrix[6] = 0.0f; matrix[7] = 0.0f;
	matrix[8] = 0.0f; matrix[9] = 0.0f; matrix[10] = 1.0f; matrix[11] = 0.0f;
	matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
}

Matrix4::Matrix4(float rawArray[])
{
	for (int i = 0; i < 16; i++)
	{
		matrix[i] = rawArray[i];
	}
}

Matrix4::Matrix4(float x0, float y0, float z0, float w0,
				 float x1, float y1, float z1, float w1,
				 float x2, float y2, float z2, float w2,
				 float x3, float y3, float z3, float w3)
{
	matrix[0] = x0; matrix[1] = y0; matrix[2] = z0; matrix[3] = w0;
	matrix[4] = x1; matrix[5] = y1; matrix[6] = z1; matrix[7] = w1;
	matrix[8] = x2; matrix[9] = y2; matrix[10] = z2; matrix[11] = w2;
	matrix[12] = x3; matrix[13] = y3; matrix[14] = z3; matrix[15] = w3;
}

Matrix4::Matrix4(Vector3 one,
				 Vector3 two,
				 Vector3 three,
				 Vector3 four)
{
	matrix[0] = one.getX();  matrix[1] = one.getY();  matrix[2] = one.getZ();  matrix[3] = 0;
	matrix[4] = two.getX();  matrix[5] = two.getY();  matrix[6] = two.getZ();  matrix[7] = 0;
	matrix[8] = three.getX();  matrix[9] = three.getY();  matrix[10] = three.getZ(); matrix[11] = 0;
	matrix[12] = four.getX(); matrix[13] = four.getY(); matrix[14] = four.getZ(); matrix[15] = 0;
}

Matrix4::Matrix4(Vector4 one,
				 Vector4 two,
				 Vector4 three,
				 Vector4 four)
{
	matrix[0] = one.getX();  matrix[1] = one.getY();  matrix[2] = one.getZ();  matrix[3] = one.getW();
	matrix[4] = two.getX();  matrix[5] = two.getY();  matrix[6] = two.getZ();  matrix[7] = two.getW();
	matrix[8] = three.getX();  matrix[9] = three.getY();  matrix[10] = three.getZ(); matrix[11] = three.getW();
	matrix[12] = four.getX(); matrix[13] = four.getY(); matrix[14] = four.getZ(); matrix[15] = four.getW();
}

//Accessors
float* Matrix4::getAsArray(){ return matrix; }

//Operators
Matrix4 Matrix4::operator*(Matrix4 m)
{
	float* mat = m.getAsArray();

	float product[16];
	int index = 0;

	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 4; row++)
		{
			float value = (matrix[(col * 4)] * mat[row]) +
									 (matrix[(col * 4) + 1] * mat[row + 4]) + 
									 (matrix[(col * 4) + 2] * mat[row + 8]) + 
									 (matrix[(col * 4) + 3] * mat[row + 12]);
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

	x = (matrix[0] * vec.getX()) + (matrix[4] * vec.getY()) + (matrix[8] * vec.getZ()) + (matrix[12] * 1.0f);
	y = (matrix[1] * vec.getX()) + (matrix[5] * vec.getY()) + (matrix[9] * vec.getZ()) + (matrix[13] * 1.0f);
	z = (matrix[2] * vec.getX()) + (matrix[6] * vec.getY()) + (matrix[10] * vec.getZ()) + (matrix[14] * 1.0f);
	
	return Vector3(x, y, z);
}

Vector4 Matrix4::operator*(Vector4 vec)
{
	float x, y, z, w;

	x = (matrix[0] * vec.getX()) + (matrix[4] * vec.getY()) + (matrix[8] * vec.getZ()) + (matrix[12] * vec.getW());
	y = (matrix[1] * vec.getX()) + (matrix[5] * vec.getY()) + (matrix[9] * vec.getZ()) + (matrix[13] * vec.getW());
	z = (matrix[2] * vec.getX()) + (matrix[6] * vec.getY()) + (matrix[10] * vec.getZ()) + (matrix[14] * vec.getW());
	w = (matrix[3] * vec.getX()) + (matrix[7] * vec.getY()) + (matrix[11] * vec.getZ()) + (matrix[15] * vec.getW());

	return Vector4(x,y,z,w);
}


Matrix4::~Matrix4(void)
{
	
}
