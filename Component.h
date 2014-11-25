#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component
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