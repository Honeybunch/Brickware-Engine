#define BRICKWARE_MATH_EXPORTS

#include "BrickwareMath\Quaternion.hpp"

using namespace Brickware;
using namespace Math;

/*
	Constructors
*/

Quaternion::Quaternion()
{
	q[0] = 0;
	q[1] = 0;
	q[2] = 0;
	q[3] = 1;
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	q[0] = x;
	q[1] = y;
	q[2] = z;
	q[3] = w;
}

//Assuming from euler angles
Quaternion::Quaternion(Vector3 v)
{
	//Attitude = X rotation = pitch
	//Heading = Y rotation = yaw
	//Bank = Z rotation = roll

	float attitudeCos = cosf(v[0] / 2.0f);
	float attitudeSin = sinf(v[0] / 2.0f);

	float headingCos = cosf(v[1] / 2.0f);
	float headingSin = sinf(v[1] / 2.0f);

	float bankCos = cosf(v[2] / 2.0f);
	float bankSin = sinf(v[2] / 2.0f);

	//we need to multiply a bunch of quaternions together for each axis
	Quaternion xQ(attitudeSin, 0, 0, attitudeCos);
	Quaternion yQ(0, headingSin, 0, headingCos);
	Quaternion zQ(0, 0, bankSin, bankCos);
	
	//Note: this order matters!
	Quaternion product = xQ * yQ * zQ;

	q[0] = product[0];
	q[1] = product[1];
	q[2] = product[2];
	q[3] = product[3];

	normalize();
}

//Assuming from Angle-Axis
Quaternion::Quaternion(Vector4 v)
{
	float halfAngle = v[3] / 2.0f;

	q[0] = v[0] * sinf(halfAngle);
	q[1] = v[1] * sinf(halfAngle);
	q[2] = v[2] * sinf(halfAngle);
	q[3] = cosf(halfAngle);
}

Quaternion Quaternion::getInverse()
{
	return Quaternion(-q[0], -q[1], -q[2], q[3]);
}

Quaternion Quaternion::getQuaternionIdentity()
{
	return Quaternion(0, 0, 0, 1);
}

Vector4 Quaternion::getAxisAngle()
{
	float xsq = powf(q[0], 2);
	float ysq = powf(q[1], 2);
	float zsq = powf(q[2], 2);

	float scale = sqrtf(xsq + ysq + zsq);

	float x = q[0] / scale;
	float y = q[1] / scale;
	float z = q[2] / scale;
	float angle = acos(q[3]) * 2.0f;

	return Vector4(x, y, z, angle);
}

Matrix3 Quaternion::getRotationMatrix()
{
	float x = q[0];
	float y = q[1];
	float z = q[2];
	float w = q[3];

	float xsq = powf(x, 2);
	float ysq = powf(y, 2);
	float zsq = powf(z, 2);
	float wsq = powf(w, 2);

	float m00 = 1 - (2 * (ysq + zsq));
	float m11 = 1 - (2 * (xsq + zsq));
	float m22 = 1 - (2 * (xsq + ysq));

	float m01 = 2 * ((x * y) + (z * w));
	float m02 = 2 * ((x * z) - (y * w));

	float m10 = 2 * ((x * y) - (z * w));
	float m12 = 2 * ((y * z) + (x * w));

	float m20 = 2 * ((x * z) + (y * w));
	float m21 = 2 * ((y * z) - (x * w));

	Matrix3 rotationMat(m00, m01, m02,
						m10, m11, m12,
						m20, m21, m22);

	return rotationMat;
}

void Quaternion::normalize()
{
	float xsq = powf(q[0], 2);
	float ysq = powf(q[1], 2);
	float zsq = powf(q[2], 2);
	float wsq = powf(q[3], 2);

	float magSq = (xsq + ysq + zsq + wsq);
	if (fabsf(magSq) > 0.00001f &&
		fabsf(magSq - 1.0f) > 0.00001f)
	{
		float mag = sqrtf(magSq);
		q[0] /= mag;
		q[1] /= mag;
		q[2] /= mag;
		q[3] /= mag;
	}
}

/*
	Accessors and Mutators
*/

float Quaternion::getX(){ return q[0]; }
float Quaternion::getY(){ return q[1]; }
float Quaternion::getZ(){ return q[2]; }
float Quaternion::getW(){ return q[3]; }

void Quaternion::setX(float x){ q[0] = x; }
void Quaternion::setY(float y){ q[1] = y; }
void Quaternion::setZ(float z){ q[2] = z; }
void Quaternion::setW(float w){ q[3] = w; }

/*
	Operators
*/

float& Quaternion::operator[](int i)
{
	return q[i];
}

Quaternion Quaternion::operator*(Quaternion other)
{
	Quaternion product;

	Vector3 qVec(q[0], q[1], q[2]);
	Vector3 pVec(other[0], other[1], other[2]);

	float qScalar = q[3];
	float pScalar = other[3];

	Vector3 productVec = (qVec * pScalar) + (pVec * qScalar) + (Vector3::Cross(pVec, qVec));
	float productScalar = (qScalar * pScalar) - Vector3::Dot(pVec, qVec);

	product[0] = productVec[0];
	product[1] = productVec[1];
	product[2] = productVec[2];
	product[3] = productScalar;

	return product;
}

//Extraction
std::ostream& operator<<(std::ostream& output, Quaternion& q)
{
	output << q[0] << "," << q[1] << "," << q[2] << "," << q[3];
	return output;
}

//Insertion
std::istream& operator>> (std::istream& input, Quaternion& q)
{
	input >> q[0] >> q[1] >> q[2] >> q[3];
	return input;
}

/*
	Destructors
*/

Quaternion::~Quaternion(void)
{
}
