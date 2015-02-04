#include "vector4.h"
#include "Vector3.h"

/*
	Constructors
*/

Vector4::Vector4()
{
	this->vector[0] = 0;
	this->vector[1] = 0;
	this->vector[2] = 0;
	this->vector[3] = 0;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	this->vector[0] = x;
	this->vector[1] = y;
	this->vector[2] = z;
	this->vector[3] = w;
}

/*
	Destructor
*/

Vector4::~Vector4()
{

}

/*
	Accessors and Mutators
*/

float Vector4::getX(){ return vector[0]; }
float Vector4::getY(){ return vector[1]; }
float Vector4::getZ(){ return vector[2]; }
float Vector4::getW(){ return vector[3]; }

float* Vector4::getAsArray(){ return vector; }

void Vector4::setX(float x){ this->vector[0] = x; }
void Vector4::setY(float y){ this->vector[1] = y; }
void Vector4::setZ(float z){ this->vector[2] = z; }
void Vector4::setW(float w){ this->vector[3] = w; }

/*
	Operators
*/

//Extraction
ostream& operator<<(ostream& output, Vector4& v4)
{
	output << v4.getX() << endl << v4.getY() << endl << v4.getZ() << endl << v4.getW() << endl;
	return output;
}

//Insertion
istream& operator>> (istream& input, Vector4& v4)
{
	input >> v4.vector[0] >> v4.vector[1] >> v4.vector[2] >> v4.vector[3];
	return input;
}

//Typecasting Operators

Vector4::operator Vector3()
{
	if (vector[3] == 0)
		vector[3] = 1.0f;

	return Vector3(vector[0] / vector[3], vector[1] / vector[3], vector[2] / vector[3]);
}