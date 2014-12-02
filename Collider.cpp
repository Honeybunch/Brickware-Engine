#include "Collider.h"

Collider::Collider()
{
	enabled = true;
	isTrigger = false;
}

bool Collider::isColliding(Collider* other){ return false; }

Collider::~Collider()
{
}
