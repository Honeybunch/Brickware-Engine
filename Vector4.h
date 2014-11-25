#ifndef VECTOR4_H
#define VECTOR4_H

#include <iostream>
using namespace std;

class Vector3;

class Vector4
{
public: 
	//Constructors
	Vector4();
	Vector4(float x, float y, float z, float w);

	//Destructor
	~Vector4();
	
	//Accessors and mutators
	float getX();
	float getY();
	float getZ();
	float getW();

	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setW(float w);

	friend ostream& operator<< (ostream& output, Vector4& v4);
	friend istream& operator>> (istream& input, Vector4& v4);

	operator Vector3();

private:
	float x, y, z, w;

};

#endif