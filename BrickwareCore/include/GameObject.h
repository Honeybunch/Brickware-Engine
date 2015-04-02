#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

//DLL Header
#include "BrickwareCoreDLL.h"

//Other Brickware Project Headers
#include "Mesh.h"

//System Level Headers
#include <vector>
#include <typeinfo>

//Project Headers
#include "Settings.h"
#include "Component.h"
#include "Transform.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "MeshRenderer.h"

namespace Brickware
{
	namespace Core
	{
		class Camera;

		template class BRICKWARE_CORE_API std::vector < GameObject* > ;
		template class BRICKWARE_CORE_API std::vector < Component* >;

		class BRICKWARE_CORE_API GameObject
		{
		public:

			GameObject();
			GameObject(Transform* transform);
			GameObject(GameObject& other);

			static std::vector<GameObject*> getGameObjects();

			//Accessors
			Transform* getTransform();

			vector<Component*>& getComponents();
			//Return the first component of the given type
			//If none are found; return null
			template<class T> T* getComponent()
			{
				for (unsigned int i = 0; i < components.size(); i++)
				{
					T* type = dynamic_cast<T*>(components[i]);
					if (type)
						return type;
				}

				return NULL;
			}

			//Mutators 
			void addComponent(Component* newComponent);

			virtual void Start();
			virtual void Update();
			virtual void FixedUpdate();
			virtual void OnCollisionEnter(Collision* collision);
			virtual void OnRender();

			~GameObject();

		protected:
			//Necessary Components
			Transform* transform;

			//Other Components
			std::vector<Component*> components;
		};
	}
}
#endif
