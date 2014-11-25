#include "Quaternion.h"

/*
	Constructors
*/

Quaternion::Quaternion()
{
	w = 0;
	x = 0;
	y = 0;
	z = 0;
}

Quaternion::Quaternion(float w, float x, float y, float z)
{
	Quaternion::w = w;
	Quaternion::x = x;
	Quaternion::y = y;
	Quaternion::z = z;
}

Quaternion::Quaternion(Vector3 v)
{
	Quaternion::w = 0;
	Quaternion::x = v.getX();
	Quaternion::y = v.getY();
	Quaternion::z = v.getZ();
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
ostream& operator<<(ostream& output, Quaternion& q)
{
	output << q.getW() << endl << q.getX() << endl << q.getY() << endl << q.getZ() << endl;
	return output;
}

//Insertion
istream& operator>> (istream& input, Quaternion& q)
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
