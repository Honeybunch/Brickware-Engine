#include "vector2.h"

/*
	Constructors
*/
Vector2::Vector2()
{
	this->vector[0] = 0;
	this->vector[1] = 0;
}

Vector2::Vector2(float x, float y)
{
	this->vector[0] = x;
	this->vector[1] = y;
}

Vector2::Vector2(const Vector2& other)
{
	this->vector[0] = other.vector[0];
	this->vector[1] = other.vector[1];
}


/*
	Destructor
*/

Vector2::~Vector2()
{

}

/*
	Accessors & Mutators
*/

float Vector2::getX(){ return vector[0]; }
float Vector2::getY(){ return vector[1]; }

float* Vector2::getAsArray(){ return vector; }

void Vector2::setX(float x){ vector[0] = x; }
void Vector2::setY(float y){ vector[1] = y; }

/*
	Static Functions
*/

//Dot product between two vectors
float Vector2::Dot(Vector2 v, Vector2 u)
{
	float output;

	output = v * u;

	return output;
}

//Sum two vectors together
Vector2 Vector2::Sum(Vector2 u, Vector2 v)
{
	Vector2 output;

	output = v + u;

	return output;
}

//Multiply a vector by a scalar
Vector2 Vector2::ScalarProduct(Vector2 v, float s)
{
	Vector2 output;

	output = v * s;

	return output;
}

//Normalize a vector
Vector2 Vector2::Normalize(Vector2 v)
{
	float magnitude = sqrt((v.vector[0] * v.vector[0]) + (v.vector[1] * v.vector[1]));

	Vector2 normalizedVec(v.vector[0] / magnitude,
						  v.vector[1] / magnitude);

	return normalizedVec;
}

/*
	Operators
*/

//Vector product
float Vector2::operator* (Vector2 u)
{
	return (vector[0] * u.vector[0] + vector[1] * u.vector[1]);
}

//Scalar product
Vector2 Vector2::operator*(float s)
{
	return Vector2(s * vector[0], s * vector[1]);
}

//Sum
Vector2 Vector2::operator+(Vector2 u)
{
	return Vector2(vector[0] + u.vector[0], vector[1] + u.vector[1]);
}

//Extraction
ostream& operator<<(ostream& output, Vector2& v)
{
	output <<  v.getX() << endl << v.getY() << endl;
	return output;
}

//Insertion
istream& operator>>(istream& input, Vector2& v)
{
	input >> v.vector[0] >> v.vector[1];
	return input;
}
