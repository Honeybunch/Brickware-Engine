#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "BrickwareCoreDLL.h"

#include <vector>
#include <typeinfo>

#include "Settings.h"

#include "Component.h"
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"
#include "Collision.h"

class Camera;

class GameObject
{
public:
	
	BRICKWARE_CORE_API GameObject();
	BRICKWARE_CORE_API GameObject(Transform* transform);
	BRICKWARE_CORE_API GameObject(GameObject& other);

	static BRICKWARE_CORE_API vector<GameObject*> getGameObjects();

	//Accessors
	BRICKWARE_CORE_API Transform* getTransform();

	BRICKWARE_CORE_API vector<Component*>& getComponents();
	//Return the first component of the given type
	//If none are found; return null
	template<class T> T* getComponent()
	{
		for (unsigned int i = 0; i < components.size(); i++)
		{
			T* type = dynamic_cast<T*>(components[i]);
			if(type)
				return type;
		}

		return NULL;
	}

	//Mutators 
	BRICKWARE_CORE_API void addComponent(Component* newComponent);

	virtual BRICKWARE_CORE_API void Start();
	virtual BRICKWARE_CORE_API void Update();
	virtual BRICKWARE_CORE_API void FixedUpdate();
	virtual BRICKWARE_CORE_API void OnCollisionEnter(Collision* collision);
	virtual BRICKWARE_CORE_API void OnRender();

	BRICKWARE_CORE_API ~GameObject();

protected:
	//Necessary Components
	Transform* transform;

	//Other Components
	vector<Component*> components;
};

#endif
