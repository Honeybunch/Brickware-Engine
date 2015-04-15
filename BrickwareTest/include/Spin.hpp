#ifndef SPIN_H
#define SPIN_H

#include "BrickwareCore\Component.hpp"
#include "BrickwareCore\Transform.hpp"
#include "BrickwareCore\GameObject.hpp"

#include "BrickwareMath\Vector3.hpp"

class Spin : public Brickware::Core::Component
{
public:
	Spin();

	Brickware::Math::Vector3 rotationDelta;

	virtual void Start();
	virtual void Update();

	~Spin();

private:
	Brickware::Core::Transform* transform;
};

#endif