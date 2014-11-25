#include "vector2.h"

/*
	Constructors
*/
Vector2::Vector2()
{
	Vector2::x = 0;
	Vector2::y = 0;
}

Vector2::Vector2(float x, float y)
{
	Vector2::x = x;
	Vector2::y = y;
}

Vector2::Vector2(const Vector2& other)
{
	Vector2::x = other.x;
	Vector2::y = other.y;
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

float Vector2::getX(){return x;}
float Vector2::getY(){return y;}

void Vector2::setX(float x){Vector2::x = x;}
void Vector2::setY(float y){Vector2::y = y;}

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
	float magnitude = sqrt((v.x*v.x) + (v.y*v.y));

	Vector2 normalizedVec(v.x / magnitude, 
						  v.y / magnitude);

	return normalizedVec;
}

/*
	Operators
*/

//Vector product
float Vector2::operator* (Vector2 u)
{
	return (x * u.x + y * u.y);
}

//Scalar product
Vector2 Vector2::operator*(float s)
{
	return Vector2(s * x, s * y);
}

//Sum
Vector2 Vector2::operator+(Vector2 u)
{
	return Vector2(x + u.x, y + u.y);
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
	input >> v.x >> v.y;
	return input;
}
