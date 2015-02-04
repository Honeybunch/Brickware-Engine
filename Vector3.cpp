#include "Vector3.h"

/*
	Constructors
*/
Vector3::Vector3()
{
	this->vector[0] = 0;
	this->vector[1] = 0;
	this->vector[2] = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	this->vector[0] = x;
	this->vector[1] = y;
	this->vector[2] = z;
}

Vector3::Vector3(const Vector3& other)
{
	this->vector[0] = other.vector[0];
	this->vector[1] = other.vector[1];
	this->vector[2] = other.vector[2];
}

Vector3::Vector3(Vector4& v4)
{
	this->vector[0] = v4.getX() / v4.getW();
	this->vector[1] = v4.getY() / v4.getW();
	this->vector[2] = v4.getZ() / v4.getW();
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

float Vector3::getX(){ return vector[0]; }
float Vector3::getY(){ return vector[1]; }
float Vector3::getZ(){ return vector[2];}

float* Vector3::getAsArray(){ return vector; }

void Vector3::setX(float x){ Vector3::vector[0] = x; }
void Vector3::setY(float y){ Vector3::vector[1] = y; }
void Vector3::setZ(float z){ Vector3::vector[2] = z; }

/*
	Static Functions
*/

//Cross product between vectors
Vector3 Vector3::Cross(Vector3 v, Vector3 u)
{
	Vector3 output;

	output.vector[0] = v.vector[1] * u.vector[2] - v.vector[2] * u.vector[1];
	output.vector[1] = v.vector[2] * u.vector[0] - v.vector[0] * u.vector[2];
	output.vector[2] = v.vector[0] * u.vector[1] - v.vector[1] * u.vector[0];

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
	float magnitude = sqrt((v.vector[0] * v.vector[0]) + (v.vector[1] * v.vector[1]) + (v.vector[2] * v.vector[2]));

	Vector3 normalizedVec(v.vector[0] / magnitude,
					      v.vector[1] / magnitude,
					      v.vector[2] / magnitude);

	return normalizedVec;
}

/*
	Operators
*/

//Vector product
float Vector3::operator* (Vector3 u)
{
	return (vector[0] * u.vector[0] + vector[1] * u.vector[1] + vector[2] * u.vector[2]);
}

//Scalar product
Vector3 Vector3::operator*(float s)
{
	return Vector3(s * vector[0], s * vector[1], s * vector[2]);
}

//Sum
Vector3 Vector3::operator+(Vector3 u)
{
	return Vector3(vector[0] + u.vector[0], vector[1] + u.vector[1], vector[2] + u.vector[2]);
}

//Difference
Vector3 Vector3::operator-(Vector3 u)
{
	return Vector3(vector[0] - u.vector[0], vector[1] - u.vector[1], vector[2] - u.vector[2]);
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
	input >> v.vector[0] >> v.vector[1] >> v.vector[2];
	return input;
}

//Casting to Vector4
Vector3::operator Vector4()
{
	return Vector4(vector[0], vector[1], vector[2], 1.0f);
}
