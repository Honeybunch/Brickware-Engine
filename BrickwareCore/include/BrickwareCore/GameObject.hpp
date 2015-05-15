#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareGraphics/Mesh.hpp"

//System Level Headers
#include <vector>
#include <typeinfo>

//Project Headers
#include "BrickwareCore/Settings.hpp"
#include "BrickwareCore/Component.hpp"
#include "BrickwareCore/Transform.hpp"
#include "BrickwareCore/Collision.hpp"
#include "BrickwareCore/Rigidbody.hpp"
#include "BrickwareCore/MeshRenderer.hpp"

namespace Brickware
{
	namespace Core
	{
		class Camera;

#ifdef _WIN32
		template class BRICKWARE_CORE_API std::vector < GameObject* > ;
		template class BRICKWARE_CORE_API std::vector < Component* >;
#endif

		/* The Basic Entity in the game scene.
		 *
		 * GameObject should not be overridden; it works just like GameObjects
		 * in Unity where you attach <Component>s to them to extend their functionality.
		 */
		class BRICKWARE_CORE_API GameObject
		{
			friend class Game;
			friend class GameInputManager;

		public:
			/* Static function for releasing game objects (DO NOT JUST DELETE GAME OBJECT POINTERS)
			* @gameObject The game object that you want to destroy.
			*/
			static void Destroy(GameObject* gameObject);

			/* Gets all the game objects in the scene.
			* @returns The static vector of all the game objects in the scene.
			*/
			static std::vector<GameObject*> getGameObjects();

			//Basic Constructor; attaches a blank transform for you
			GameObject();
			//Constructor if you have a transform that you have already created
			GameObject(Transform* transform);
			//Copy Constructor
			GameObject(GameObject& other);

			/* Gets the attached <Transform> component.
			 * @returns The <Transform> component that every <GameObject> must have.
			 */
			Transform* getTransform();

			/* Gets all the attached <Component>s (including the <Transform>).
			 * @returns a vector of all the attached <Component>s.
			 */
			vector<Component*>& getComponents();

			/* Gets a <Component> of a given type.
			 * @returns The first <Component> of the given type or,
			 *          nullptr if no component of that type is found.
			 */
			template<class T> T* getComponent()
			{
				for (unsigned int i = 0; i < components.size(); i++)
				{
					T* type = dynamic_cast<T*>(components[i]);
					if (type)
						return type;
				}

				return nullptr;
			}

			/* Adds a <Component> on to this GameObject.
			 * @newComponent A pointer to the <Component> that you wnat to add
			 */
			void addComponent(Component* newComponent);

			//Calls the Start method of all attached <Component>s
			virtual void Start();
			//Calls the Update method of all attached <Component>s
			virtual void Update();
			//Calls the FixedUpdate method of all attached <Component>s
			virtual void FixedUpdate();

			/* Calls the OnCollisionEnter method of all attached <Component>s
			 * @collision The pointer to the <Collision> data that it is going to send
			 *            to the attached <Component>s
			 */
			virtual void OnCollisionEnter(Collision* collision);

			//Calls the OnMouseOver component of all attached <Component>s
			virtual void OnMouseOver();
			//Calls the OnMouseStay component of all attached <Component>s
			virtual void OnMouseStay();
			//Calls the OnMouseExit component of all attached <Component>s
			virtual void OnMouseExit();

			//Calls the Render method of all attached <Component>s
			virtual void OnRender();

			//Destructor
			~GameObject();

		protected:
			//The <Transform> of this game object
			Transform* transform;

			//All attached <Component>s
			std::vector<Component*> components;

		private:
			bool toDestroy;
		};
	}
}
#endif
