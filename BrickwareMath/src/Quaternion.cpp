#define BRICKWARE_MATH_EXPORTS

#include "Quaternion.h"

/*
	Constructors
*/

Quaternion::Quaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	Quaternion::x = x;
	Quaternion::y = y;
	Quaternion::z = z;
	Quaternion::w = w;
}

//Assuming from euler angles
Quaternion::Quaternion(Vector3 v)
{
	//Heading = y rotation
	//Attitude = x rotation
	//Bank = z rotation

	float headingCos = cosf(v[1] / 2.0f);
	float headingSin = sinf(v[1] / 2.0f);

	float attitudeCos = cosf(v[0] / 2.0f);
	float attitudeSin = sinf(v[0] / 2.0f);

	float bankCos = cosf(v[2] / 2.0f);
	float bankSin = sinf(v[2] / 2.0f);

	x = (headingCos * attitudeCos * bankSin) + (headingSin * attitudeSin * bankCos);

	y = (headingSin * attitudeCos * bankCos) + (headingCos * attitudeSin * bankSin);

	z = (headingCos * attitudeSin * bankCos) - (headingSin * attitudeCos * bankSin);

	w = (headingCos * attitudeCos * bankCos) - (headingSin * attitudeSin * bankSin);
}

Quaternion Quaternion::getQuaternionIdentity()
{
	return Quaternion(1, 0, 0, 0);
}

Vector3 Quaternion::getEulerAngles()
{
	float xsq = powf(x, 2);
	float ysq = powf(y, 2);
	float zsq = powf(z, 2);
	float wsq = powf(w, 2);

	float unit = xsq + ysq + zsq + wsq;
	float test = (x * y) + (z * w);

	float vY; //Heading
	float vX; //Attitude
	float vZ; //Bank

	//Handle singularity at north pole
	if (test > 0.499f * unit)
	{
		vY = 2 * atan2(x, w);
		vX = (float)M_PI / 2.0f;
		vZ = 0;
	}
	else if (test < -0.499f * unit)
	{
		vY = -2.0f * atan2(x, w);
		vX = (float)M_PI / -2.0f;
		vZ = 0;
	}
	else
	{
		vY = atan2f(2 * ((w * y) - (x * z)), xsq - ysq - zsq + wsq);
		vX = asinf(2 * test / unit);
		vZ = atan2f(2 * ((w * x) - (y * z)), -xsq + ysq -zsq + wsq);
	}

	return Vector3(vX, vY, vZ);
}

Matrix3 Quaternion::getRotationMatrix()
{
	float xsq = powf(x, 2);
	float ysq = powf(y, 2);
	float zsq = powf(z, 2);
	float wsq = powf(w, 2);

	float inverse = 1.0f / (xsq + ysq + zsq + wsq);

	float m00 = ( xsq - ysq - zsq + wsq) * inverse;
	float m11 = (-xsq + ysq - zsq + wsq) * inverse;
	float m22 = (-xsq - ysq + zsq + wsq) * inverse;

	float temp1 = x * y;
	float temp2 = z * w;

	float m10 = 2.0f * (temp1 + temp2) * inverse;
	float m01 = 2.0f * (temp1 - temp2) * inverse;

	temp1 = x * z;
	temp2 = y * w;

	float m20 = 2.0f * (temp1 - temp2) * inverse;
	float m02 = 2.0f * (temp1 + temp2) * inverse;

	temp1 = y * z;
	temp2 = x * w;

	float m21 = 2.0f * (temp1 + temp2) * inverse;
	float m12 = 2.0f * (temp1 - temp2) * inverse;

	Matrix3 rotationMat(m00, m01, m02,
						m10, m11, m12,
						m20, m21, m22);

	return rotationMat;
}

/*
	Accessors and Mutators
*/

float Quaternion::getW(){return w;}
float Quaternion::getX(){return x;}
float Quaternion::getY(){return y;}
float Quaternion::getZ(){return z;}

void Quaternion::setW(float w){Quaternion::w = w;}
void Quaternion::setX(float x){Quaternion::x = x;}
void Quaternion::setY(float y){Quaternion::y = y;}
void Quaternion::setZ(float z){Quaternion::z = z;}

/*
	Operators
*/

//Extraction
std::ostream& operator<<(std::ostream& output, Quaternion& q)
{
	output << q.getW() << std::endl << q.getX() << std::endl << q.getY() << std::endl << q.getZ() << std::endl;
	return output;
}

//Insertion
std::istream& operator>> (std::istream& input, Quaternion& q)
{
	input >> q.w >> q.x >> q.y >> q.z;
	return input;
}

/*
	Destructors
*/

Quaternion::~Quaternion(void)
{
}
