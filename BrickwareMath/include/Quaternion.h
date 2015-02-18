#ifndef QUATERNION_H
#define QUATERNION_H

#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>

#include "BrickwareMathDLL.h"

#include "Vector3.h"
#include "Matrix3.h"

class BRICKWARE_MATH_API Quaternion
{
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(Vector3 v);

	~Quaternion();

	static Quaternion getQuaternionIdentity();

	Vector3 getEulerAngles();
	Matrix3 getRotationMatrix();

	//Accessors and mutators
	float getX();
	float getY();
	float getZ();
	float getW();
	
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setW(float w);
	
	friend std::ostream& operator<< (std::ostream& output, Quaternion& h);
	friend std::istream& operator>> (std::istream& input, Quaternion& h);

private:
	float x, y, z, w;
};

#endif