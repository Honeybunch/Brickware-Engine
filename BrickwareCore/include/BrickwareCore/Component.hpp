#ifndef COMPONENT_H
#define COMPONENT_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareGraphics/Primitive.hpp"

//Project Headers
#include "BrickwareCore/Settings.hpp"
#include "BrickwareCore/Screen.hpp"
#include "BrickwareCore/GameTime.hpp"
#include "BrickwareCore/Collision.hpp"
#include "BrickwareCore/Input.hpp"

namespace Brickware
{
	namespace Core
	{
		class GameObject;
		class Collider;

		/* Components are containers of logic that get attached to <GameObject>s.
		 *
		 * Similar to Unity, the Component class is supposed to be extended by the user of the engine.
		 */
		class BRICKWARE_CORE_API Component
		{
		public:
			//Blank Constructor
			Component();

			/* Gets the <GameObject> that this component is attached to.
			 * @return A pointer to the <GameObject> that this Component is attached to.
			 */
			GameObject* getGameObject();

			/* Sets the <GameObject> that this component will be attached to.
			 * @gameObject The pointer to the <GameObject> that this component should attach to
			 */
			void setGameObject(GameObject* gameObject);

			/* Override this if you want to explicitly describe how this component should be copied if its parent <GameObject> is copied
			 *
			 * Since everything in Brickware isn't serialzed copying <GameObject>s becomes trickier.
			 * If the component extension you write is going to be attached to a <GameObject> that you 
			 * intend to copy at some point then you should override this Clone method to return 
			 * a pointer to a copy of your extended Component
			 *
			 * @return A pointer to a clone of this component.
			 */
			virtual Component* Clone();

			// Override this to describe what happens when your component is Started by its <GameObject>
			virtual void Start();

			/* Override this to describe what your Component will do every frame of Game Logic
			 * 
			 * Update will be called as often as possible but before <LateUpdate> and <Render>
			 */
			virtual void Update();

			/* Override this to describe what your Component will do every frame of Game Logic but after <Update>
			 *
			 * LateUpdate gets called as often as possible but after <Update> and before <Render>
			 */
			virtual void LateUpdate();

			/* Override this to describe what your Component will do once every 1/60th of a second
			 *
			 * FixedUpdate gets called 60 times per second in a tick system. Use carefully as
			 * it currently takes place in the same block of code as all Physics operations.
			 * DO NOT RELY ON THIS
			 */
			virtual void FixedUpdate();

			/* Override this to describe what your Component will do when it encounters a collision event
			 *
			 * The parent <GameObject> must have an attached <Collider> of some sort and a <Rigidbody>
			 */
			virtual void OnCollision(Collision* collision);

			/* Override this to describe what your Component will do when it encounters a Trigger event
			*
			* The parent <GameObject> must have an attached <Collider> of some sort which is marked as a trigger
			*/
			virtual void OnTrigger(Collision* collision);

			/* Override this to describe what your Component will do when the mouse is over the attached <GameObject>. 
			 * 
			 * The parent <GameObject> must have an attached <Collider>
			 */
			virtual void OnMouseOver();
			
			/* Override this to describe what your Component will do when the mouse stays hovered over the attached <GameObject>.
			 *
			 * The parent <GameObject> must have an attached <Collider>
			 */
			virtual void OnMouseStay();

			/* Override this to describe what your Component will do when the mouse stops hover over the attached <GameObject>.
			 *
			 * The parent <GameObject> must have an attached <Collider>
			 */
			virtual void OnMouseExit();

			/* Override this to describe what your Component will do every frame of Game Logic but after <Update> and <LateUpdate>
			*
			* Render will be called as often as possible. This is usually reserved for components that need to send data
			* to the GPU via a <Material>
			*/
			virtual void Render();

#ifdef _DEBUG
			//Override this if you want your component to draw something while Debug Mode is active
			virtual void DebugDraw();
#endif

			//Destructor
			virtual ~Component();

		private:
			GameObject* gameObject;
		};
	}
}
#endif