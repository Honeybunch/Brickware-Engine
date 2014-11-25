#include "SphereCollider.h"


SphereCollider::SphereCollider(Vector3* center, float radius)
	: Collider()
{
	this->center = center;
	this->radius = radius;
}

Vector3* SphereCollider::getCenter(){ return center;  }
float SphereCollider::getRadius(){ return radius; }

SphereCollider::~SphereCollider()
{
}
