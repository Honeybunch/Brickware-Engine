#define BRICKWARE_MATH_EXPORTS

#include "BrickwareMath\Vector2.hpp"

using namespace Brickware;
using namespace Math;

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

Vector2 Vector2::operator+(Vector2 u)
{
	return Vector2(vector[0] + u.vector[0], vector[1] + u.vector[1]);
}

float& Vector2::operator[](int i)
{
	return vector[i];
}

//Extraction
std::ostream& operator<<(std::ostream& output, Vector2& v)
{
	output << v.getX() << std::endl << v.getY() << std::endl;
	return output;
}

//Insertion
std::istream& operator>>(std::istream& input, Vector2& v)
{
	float x, y;
	input >> x >> y;

	v.setX(x);
	v.setY(y);

	return input;
}
