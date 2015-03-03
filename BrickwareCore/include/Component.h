#ifndef COMPONENT_H
#define COMPONENT_H

#include "BrickwareCoreDLL.h"

#include "Settings.h"
#include "Screen.h"
#include "GameTime.h"
#include "Primitive.h"

class GameObject;
class Collider;

#include "Input.h"

class BRICKWARE_CORE_API Component
{
public:
	Component();
	
	GameObject* getGameObject();
	
	void setGameObject(GameObject* gameObject);

	virtual Component* Clone();

	virtual void Start();

	virtual void Update();
	virtual void LateUpdate();
	virtual void FixedUpdate();

	virtual void OnCollision(Collider* other);
	virtual void OnTrigger(Collider* other);

	virtual void Render();

#ifdef _DEBUG
	virtual void DebugDraw();
#endif

	virtual ~Component();

private:
	GameObject* gameObject;
};

#endif