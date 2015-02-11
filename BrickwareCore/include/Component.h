#ifndef COMPONENT_H
#define COMPONENT_H

#include "Settings.h"
#include "Screen.h"

class GameObject;

#include "Input.h"

class BRICKWARE_CORE_API Component
{
public:
	Component();

	GameObject* getGameObject();
	
	void setGameObject(GameObject* gameObject);

	virtual void Start();

	virtual void Update();

	virtual void Render();

	~Component();

private:
	GameObject* gameObject;
};

#endif