/*
	The base of everything in the engine. GameObjects can have specialized behaviour 
	when components are attached to them.
*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <typeinfo>

#include "Settings.h"

#include "Component.h"
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"

class Camera;

class GameObject
{
public:
	
	GameObject();
	GameObject(Transform* transform);

	static vector<GameObject*> getGameObjects();

	//Accessors
	Transform* getTransform();

	vector<Component*>& getComponents();
	//Return the first component of the given type
	//If none are found; return null
	template<class T> T* getComponent()
	{
		for (int i = 0; i < componentCount; i++)
		{
			T* type = dynamic_cast<T*>(components[i]);
			if(type)
				return type;
		}

		return NULL;
	}

	//Mutators 
	void addComponent(Component* newComponent);

	virtual void Start();
	virtual void Update();
	virtual void OnRender();

	~GameObject();

protected:
	//Necessary Components
	Transform* transform;

	//Other Components
	vector<Component*> components;
	int componentCount; 
};

#endif
