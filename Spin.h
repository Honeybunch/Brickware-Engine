#ifndef SPIN_H
#define SPIN_H

#include "Component.h"
#include "Transform.h"
#include "Vector3.h"
#include "GameObject.h"

class Spin :
	public Component
{
public:
	Spin();

	Vector3 rotationDelta;

	virtual void Start();
	virtual void Update();

	~Spin();

private:
	Transform* transform;
};

#endif