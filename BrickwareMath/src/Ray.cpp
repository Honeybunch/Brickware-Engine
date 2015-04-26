#define BRICKWARE_MATH_EXPORTS

#include "BrickwareMath\Ray.hpp"

using namespace Brickware;
using namespace Math;

Ray::Ray(Vector3 direction)
{
	this->origin = Vector3();
	this->direction = direction;
}

Ray::Ray(Vector3 origin, Vector3 direction)
{
	this->origin = origin;
	this->direction = direction;
}

Vector3 Ray::getOrigin(){ return origin; }
Vector3 Ray::getDirection(){ return direction; }

void Ray::setOrigin(Vector3 origin){ this->origin = origin; }
void Ray::setDirection(Vector3 direction){ this->direction = direction; }