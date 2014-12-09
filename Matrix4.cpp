#include "Matrix4.h"


Matrix4::Matrix4()
{
	matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f; matrix[3] = 0.0f;
	matrix[4] = 0.0f; matrix[5] = 1.0f; matrix[6] = 0.0f; matrix[7] = 0.0f;
	matrix[8] = 0.0f; matrix[9] = 0.0f; matrix[10] = 1.0f; matrix[11] = 0.0f;
	matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
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
	matrix[0] = one.getX(); matrix[1] = one.getY(); matrix[2] = one.getZ(); matrix[3] = 0;
	matrix[4] = two.getX(); matrix[5] = two.getY(); matrix[6] = two.getZ(); matrix[7] = 0;
	matrix[8] = three.getX(); matrix[9] = three.getY(); matrix[10] = three.getZ(); matrix[11] = 0;
	matrix[12] = four.getX(); matrix[13] = four.getY(); matrix[14] = four.getZ(); matrix[15] = 0;
}

Matrix4::Matrix4(Vector4 one,
				 Vector4 two,
				 Vector4 three,
				 Vector4 four)
{
	matrix[0] = one.getX(); matrix[1] = one.getY(); matrix[2] = one.getZ(); matrix[3] = one.getW();
	matrix[4] = two.getX(); matrix[5] = two.getY(); matrix[6] = two.getZ(); matrix[7] = two.getW();
	matrix[8] = three.getX(); matrix[9] = three.getY(); matrix[10] = three.getZ(); matrix[11] = three.getW();
	matrix[12] = four.getX(); matrix[13] = four.getY(); matrix[14] = four.getZ(); matrix[15] = four.getW();
}

//Accessors
float* Matrix4::getAsArray(){ return matrix; }

//Operators
Matrix4 Matrix4::operator*(Matrix4 m)
{
	float* mat = m.getAsArray();

	float x0, y0, z0, w0;
	float x1, y1, z1, w1;
	float x2, y2, z2, w2;
	float x3, y3, z3, w3;

	x0 = (matrix[0] * mat[0]) + (matrix[1] * mat[4]) + (matrix[2] * mat[8]) + (matrix[3] * mat[12]);
	x1 = (matrix[0] * mat[1]) + (matrix[1] * mat[5]) + (matrix[2] * mat[9]) + (matrix[3] * mat[13]);
	x2 = (matrix[0] * mat[2]) + (matrix[1] * mat[6]) + (matrix[2] * mat[10]) + (matrix[3] * mat[14]);
	x3 = (matrix[0] * mat[3]) + (matrix[1] * mat[7]) + (matrix[2] * mat[11]) + (matrix[3] * mat[15]);

	y0 = (matrix[4] * mat[0]) + (matrix[5] * mat[4]) + (matrix[6] * mat[8]) + (matrix[7] * mat[12]);
	y1 = (matrix[4] * mat[1]) + (matrix[5] * mat[5]) + (matrix[6] * mat[9]) + (matrix[7] * mat[13]);
	y2 = (matrix[4] * mat[2]) + (matrix[5] * mat[6]) + (matrix[6] * mat[10]) + (matrix[7] * mat[14]);
	y3 = (matrix[4] * mat[3]) + (matrix[5] * mat[7]) + (matrix[6] * mat[11]) + (matrix[7] * mat[15]);

	z0 = (matrix[8] * mat[0]) + (matrix[9] * mat[4]) + (matrix[10] * mat[8]) + (matrix[11] * mat[12]);
	z1 = (matrix[8] * mat[1]) + (matrix[9] * mat[5]) + (matrix[10] * mat[9]) + (matrix[11] * mat[13]);
	z2 = (matrix[8] * mat[2]) + (matrix[9] * mat[6]) + (matrix[10] * mat[10]) + (matrix[11] * mat[14]);
	z3 = (matrix[8] * mat[3]) + (matrix[9] * mat[7]) + (matrix[10] * mat[11]) + (matrix[11] * mat[15]);

	w0 = (matrix[12] * mat[0]) + (matrix[13] * mat[4]) + (matrix[14] * mat[8]) + (matrix[15] * mat[12]);
	w1 = (matrix[12] * mat[1]) + (matrix[13] * mat[5]) + (matrix[14] * mat[9]) + (matrix[15] * mat[13]);
	w2 = (matrix[12] * mat[2]) + (matrix[13] * mat[6]) + (matrix[14] * mat[10]) + (matrix[15] * mat[14]);
	w3 = (matrix[12] * mat[3]) + (matrix[13] * mat[7]) + (matrix[14] * mat[11]) + (matrix[15] * mat[15]);

	Matrix4 newMat(x0, y0, z0, w0,
				   x1, y1, z1, w1,
				   x2, y2, z2, w2,
				   x3, y3, z3, w3);

	return newMat;
}

Vector3 Matrix4::operator*(Vector3 vec)
{
	Vector4 vec4(vec);

	return Vector3((*this) * vec4);
}

Vector4 Matrix4::operator*(Vector4 vec)
{
	float x, y, z, w;

	x = (matrix[0] * vec.getX()) + (matrix[1] * vec.getY()) + (matrix[2] * vec.getZ()) + (matrix[3] * vec.getW());
	y = (matrix[4] * vec.getX()) + (matrix[5] * vec.getY()) + (matrix[6] * vec.getZ()) + (matrix[7] * vec.getW());
	z = (matrix[8] * vec.getX()) + (matrix[9] * vec.getY()) + (matrix[10] * vec.getZ()) + (matrix[11] * vec.getW());
	w = (matrix[12] * vec.getX()) + (matrix[13] * vec.getY()) + (matrix[14] * vec.getZ()) + (matrix[15] * vec.getW());

	return Vector4(x,y,z,w);
}


Matrix4::~Matrix4(void)
{
	
}
