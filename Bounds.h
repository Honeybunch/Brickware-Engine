#ifndef BOUNDS_H
#define BOUNDS_H

#include "Vector3.h"

class Bounds
{
public:
	Bounds(Vector3 center, float width);
	Bounds(Vector3 center, float xWidth, float yWidth, float zWidth);
	~Bounds();

	//Accessors and Mutators
	float getXWidth();
	float getYWidth();
	float getZWidth();

	Vector3 getCenter();

	Vector3 getMinBound();
	Vector3 getMaxBound();

	void setMinBound(Vector3 minBound);
	void setMaxBound(Vector3 maxBound);

	//For collisions
	Vector3 getPositive(Vector3 normal);
	Vector3 getNegative(Vector3 normal);

	bool isCollidingWithBounds(Bounds* other);

private:
	float xWidth;
	float yWidth;
	float zWidth;

	Vector3 center;

	Vector3 minBound;
	Vector3 maxBound;
};

#endif