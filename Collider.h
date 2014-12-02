#ifndef COLLIDER_H
#define COLLIDER_H

#include <vector>

#include "Component.h"
#include "Triangle.h"

class Collider : public Component
{
public:
	Collider();

	virtual bool isColliding(Collider* collider);

	~Collider();

protected:
	bool enabled;
	bool isTrigger;
};

#endif