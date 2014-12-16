#include "vector4.h"
#include "Vector3.h"

/*
	Constructors
*/

Vector4::Vector4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	Vector4::x = x;
	Vector4::y = y;
	Vector4::z = z;
	Vector4::w = w;
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

float Vector4::getX(){return x;}
float Vector4::getY(){return y;}
float Vector4::getZ(){return z;}
float Vector4::getW(){return w;}

void Vector4::setX(float x){Vector4::x = x;}
void Vector4::setY(float y){Vector4::y = y;}
void Vector4::setZ(float z){Vector4::z = z;}
void Vector4::setW(float w){Vector4::w = w;}

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
	input >> v4.x >> v4.y >> v4.z >> v4.w;
	return input;
}

//Typecasting Operators

Vector4::operator Vector3()
{
	if (w == 0)
		w = 1.0f;

	return Vector3(x / w, y / w, z / w);
}