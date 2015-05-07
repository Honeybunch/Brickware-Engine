#ifndef FIRST_PERSON_CONTROLLER_H
#define FIRST_PERSON_CONTROLLER_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareMath/Vector3.hpp"

//Project Headers
#include "BrickwareCore/Component.hpp"
#include "BrickwareCore/GameObject.hpp"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API FirstPersonController : public Component
		{
		public:
			FirstPersonController();

			//Moves the Camera's <GameObject> forward
			void moveForward();
			//Moves the Camera's <GameObject> backward
			void moveBackward();
			//Strafes the Camera's <GameObject> left
			void moveLeft();
			//Strafes the Camera's <GameObject> right
			void moveRight();

			/* For setting the speed of the controller's movement.
			 * @newSpeed The new speed to set the controller's movement to.
			 */
			void setSpeed(float newSpeed);

			//Handles movement and mouse input
			virtual void Update() override;

		private:
			float speed;
		};
	}
}
#endif