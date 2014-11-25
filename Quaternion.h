#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>

#include "Vector3.h"

using namespace std;

class Quaternion
{
public:
	Quaternion();
	Quaternion(float w, float x, float y, float z);
	Quaternion(Vector3 v);

	~Quaternion();

	//Accessors and mutators
	float getW();
	float getX();
	float getY();
	float getZ();
	
	void setW(float w);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	
	friend ostream& operator<< (ostream& output, Quaternion& h);
	friend istream& operator>> (istream& input, Quaternion& h);

private:
	float w, x, y, z;
};

#endif