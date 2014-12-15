#include "Bounds.h"

Bounds::Bounds(Vector3 center, float width)
{
	this->center = center;

	xWidth = width;
	yWidth = width;
	zWidth = width;

	float halfWidth = width / 2.0f;

	minBound = Vector3(center.getX() - halfWidth, center.getY() - halfWidth, center.getZ() - halfWidth);
	maxBound = Vector3(center.getX() + halfWidth, center.getY() + halfWidth, center.getZ() + halfWidth);
}

Bounds::Bounds(Vector3 center, float xWidth, float yWidth, float zWidth)
{
	this->center = center;

	this->xWidth = xWidth; 
	this->yWidth = yWidth;
	this->zWidth = zWidth;

	minBound = Vector3(center.getX() - xWidth/2, center.getY() - yWidth/2, center.getZ() - zWidth/2);
	maxBound = Vector3(center.getX() + xWidth/2, center.getY() + yWidth/2, center.getZ() + zWidth/2);
}

Bounds::Bounds(Vector3 minBound, Vector3 maxBound)
{
	xWidth = maxBound.getX() - minBound.getX();
	yWidth = maxBound.getY() - minBound.getY();
	zWidth = maxBound.getZ() - minBound.getZ();

	this->center = Vector3(xWidth/2, yWidth/2, zWidth/2);

	this->minBound = minBound;
	this->maxBound = maxBound;
}

//Accessors and Mutators
float Bounds::getXWidth(){ return xWidth; }
float Bounds::getYWidth(){ return yWidth; }
float Bounds::getZWidth(){ return zWidth; }

Vector3 Bounds::getCenter(){ return center; }

Vector3 Bounds::getMinBound(){ return minBound; }
Vector3 Bounds::getMaxBound(){ return maxBound; }

void Bounds::setMinBound(Vector3 minBound){ this->minBound = minBound; }
void Bounds::setMaxBound(Vector3 maxBound){ this->maxBound = maxBound; }

//For collisions
Vector3 Bounds::getPositive(Vector3 normal)
{
	Vector3 positive(minBound);

	if (normal.getX() >= 0)
		positive.setX(maxBound.getX());
	
	if (normal.getY() >= 0)
		positive.setY(maxBound.getY());
	
	if (normal.getZ() >= 0)
		positive.setZ(maxBound.getZ());

	return positive;
}

Vector3 Bounds::getNegative(Vector3 normal)
{
	Vector3 negative(maxBound);

	if (normal.getX() >= 0)
		negative.setX(minBound.getX());
	
	if (normal.getY() >= 0)
		negative.setY(minBound.getY());
	
	if (normal.getZ() >= 0)
		negative.setZ(minBound.getZ());

	return negative;
}

bool Bounds::isCollidingWithBounds(Bounds* other)
{
	bool x = fabs(center.getX() - other->center.getX()) <= ((xWidth/2.0f) + (other->xWidth/2.0f));
	bool y = fabs(center.getY() - other->center.getY()) <= ((yWidth/2.0f) + (other->yWidth/2.0f));
	bool z = fabs(center.getZ() - other->center.getZ()) <= ((zWidth/2.0f) + (other->zWidth/2.0f));

	if (x == true && y == true && z == true)
		return true;
	else
		return false;
}

Bounds::~Bounds()
{
}
