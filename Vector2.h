#ifndef VECTOR2_H
#define VECTOR2_H

#include "vector2.h"

#include <iostream>
using namespace std;

class Vector2
{
public:
	//Constructors
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& other);

	//Destructor
	~Vector2();

	//Accessors & Mutators
	float getX();
	float getY();

	float* getAsArray();

	void setX(float x);
	void setY(float y);

	//Static functions
	static float Dot(Vector2 v, Vector2 u);
	static Vector2 Sum(Vector2 v, Vector2 u);
	static Vector2 ScalarProduct(Vector2 v, float s);
	static Vector2 Normalize(Vector2 v);

	//Operators
	float operator* (Vector2 u);
	Vector2 operator* (float s);
	Vector2 operator+ (Vector2 u);

	friend ostream& operator<< (ostream& output, Vector2& h);
	friend istream& operator>> (istream& input, Vector2& h);

private:
	float vector[2];
};

#endif