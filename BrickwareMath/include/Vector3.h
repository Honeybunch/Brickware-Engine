#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>

#include "BrickwareMathDLL.h"

class Vector4;

class BRICKWARE_MATH_API Vector3
{
public:
	//Constructors
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector3& other);
	Vector3(Vector4& v4);

	//Destructor
	~Vector3();

	//Accessors & Mutators
	float getX();
	float getY();
	float getZ();

	float* getAsArray();

	void setX(float x);
	void setY(float y);
	void setZ(float z);

	//Static functions
	static Vector3 Cross(Vector3 v, Vector3 u);
	static float Dot(Vector3 v, Vector3 u);
	static Vector3 Sum(Vector3 v, Vector3 u);
	static Vector3 ScalarProduct(Vector3 v, float s);
	static Vector3 Normalize(Vector3 v);

	//Operators
	Vector3 operator* (float s);
	Vector3 operator/ (float s);
	Vector3 operator- (float s);
	Vector3 operator+ (float s);

	Vector3 operator*= (float s);
	Vector3 operator/= (float s);
	Vector3 operator-= (float s);
	Vector3 operator+= (float s);

	float operator* (Vector3 u);
	Vector3 operator+ (Vector3 u);
	Vector3 operator- (Vector3 u);
	Vector3 operator+= (Vector3 u);
	Vector3 operator-= (Vector3 u);

	float& operator[] (int i);

	friend std::ostream& operator<< (std::ostream& output, Vector3& h);
	friend std::istream& operator>> (std::istream& input, Vector3& h);

	operator Vector4();

private:
	float vector[3];
};

#endif