#include "Vector3.h"

/*
	Constructors
*/
Vector3::Vector3()
{
	Vector3::x = 0;
	Vector3::y = 0;
	Vector3::z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	Vector3::x = x;
	Vector3::y = y;
	Vector3::z = z;
}

Vector3::Vector3(const Vector3& other)
{
	Vector3::x = other.x;
	Vector3::y = other.y;
	Vector3::z = other.z;
}

Vector3::Vector3(Vector4& v4)
{
	Vector3::x = v4.getX() / v4.getW();
	Vector3::y = v4.getY() / v4.getW();
	Vector3::z = v4.getZ() / v4.getW();
}


/*
	Destructor
*/

Vector3::~Vector3()
{
	//if (rawData)
	//	delete[] rawData;
}

/*
	Accessors & Mutators
*/

float Vector3::getX(){return x;}
float Vector3::getY(){return y;}
float Vector3::getZ(){return z;}

float* Vector3::getAsArray()
{
	rawData[0] = Vector3::x;
	rawData[1] = Vector3::y;
	rawData[2] = Vector3::z;

	return rawData;
}

void Vector3::setX(float x){Vector3::x = x;}
void Vector3::setY(float y){Vector3::y = y;}
void Vector3::setZ(float z){Vector3::z = z;}

/*
	Static Functions
*/

//Cross product between vectors
Vector3 Vector3::Cross(Vector3 v, Vector3 u)
{
	Vector3 output;

	output.x = v.y * u.z - v.z * u.y;
	output.y = v.z * u.x - v.x * u.z;
	output.z = v.x * u.y - v.y * u.x;

	return output;
}

//Dot product between two vectors
float Vector3::Dot(Vector3 v, Vector3 u)
{
	float output;

	output = v * u;

	return output;
}

//Sum two vectors together
Vector3 Vector3::Sum(Vector3 u, Vector3 v)
{
	Vector3 output;

	output = v + u;

	return output;
}

//Multiply a vector by a scalar
Vector3 Vector3::ScalarProduct(Vector3 v, float s)
{
	Vector3 output;

	output = v * s;

	return output;
}

//Normalize a vector
Vector3 Vector3::Normalize(Vector3 v)
{
	float magnitude = sqrt((v.x*v.x) + (v.y*v.y) + (v.z*v.z));

	Vector3 normalizedVec(v.x / magnitude, 
						  v.y / magnitude,
						  v.z / magnitude);

	return normalizedVec;
}

/*
	Operators
*/

//Vector product
float Vector3::operator* (Vector3 u)
{
	return (x * u.x + y * u.y + z * u.z);
}

//Scalar product
Vector3 Vector3::operator*(float s)
{
	return Vector3(s * x, s * y, s * z);
}

//Sum
Vector3 Vector3::operator+(Vector3 u)
{
	return Vector3(x + u.x, y + u.y, z + u.z);
}

//Difference
Vector3 Vector3::operator-(Vector3 u)
{
	return Vector3(x - u.x, y - u.y, z - u.z);
}

//Extraction
ostream& operator<<(ostream& output, Vector3& v)
{
	output <<  v.getX() << endl << v.getY() << endl << v.getZ() << endl;
	return output;
}

//Insertion
istream& operator>>(istream& input, Vector3& v)
{
	input >> v.x >> v.y >> v.z;
	return input;
}

//Casting to Vector4
Vector3::operator Vector4()
{
	return Vector4(x, y, z, 1.0f);
}
