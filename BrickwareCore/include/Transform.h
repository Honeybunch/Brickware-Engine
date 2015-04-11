#ifndef TRANSFORM_H
#define TRANSFORM_H

//DLL Header
#include "BrickwareCoreDLL.h"

//Graphics Headers
#ifdef D3D_SUPPORT
#include <windows.h>
#endif

#ifdef GL_SUPPORT
#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>
#endif

//Other Brickware Project Headers
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Quaternion.h"

#include "Shader.h"

//Project Headers
#include "Settings.h"
#include "Component.h"

namespace Brickware
{
	namespace Core
	{
		class GameObject;

		/* A <Component> that stores transform data of a <GameObject>.
		 *
		 * Every <GameObject> will have a Transform <Component> attached as it is
		 * responsible for sending certain information such as position, scale, rotation, etc.
		 * to the GameObject's attached <MeshRenderer>'s <Material>. Not the best system 
		 * but it works for now.
		 */
		class BRICKWARE_CORE_API Transform : public Component
		{
		public:
			//Blank constructor
			Transform();

			/* Gets position.
			 * @return A <Vector3> describing position.
			 */
			Math::Vector3 getPosition();

			/* Gets rotation in Euler coordinates.
			 * @return A <Vector3> describing euler rotation.
			 */
			Math::Vector3 getEulerRotation();

			/* Gets quaternion rotation.
			 * @return A <Quaternion> describing rotation.
			 */
			Math::Quaternion getRotation();
			
			/* Gets scale.
			 * @return A <Vector3> describing scale.
			 */
			Math::Vector3 getScale();

			/* Gets forward vector.
			 * @return A <Vector3> describing a forward facing normal.
			 */
			Math::Vector3 getForward();

			/* Gets right vector.
			 * @return A <Vector3> describing a right facing normal.
			 */
			Math::Vector3 getRight();

			/* Gets up vector.
			 * @return A <Vector3> describing an upwards facing normal.
			 */
			Math::Vector3 getUp();

			/* Gets matrix describing the position, rotation and scale in world space.
			 * @return A <Matrix4> describing position, rotation and scale in world space.
			 */
			Math::Matrix4 getModelMatrix();

			/* Sets the position.
			 * @newPosition A <Vector3> describing the new position.
			 */
			void setPosition(Math::Vector3 newPosition);

			/* Sets the rotation in Euler coordinates.
			 * @newRotation A <Vector3> describing the new Euler rotation.
			 * 
			 * This WILL update the rotation <Quaterion> to match.
			 */
			void setEulerRotation(Math::Vector3 newRotation);

			/* Sets the rotation.
			 * @newRotation A <Quaternion> describing the new rotation.
			 *
			 * This will NOT update the euler rotation vector to match.
			 */
			void setRotation(Math::Quaternion newRotation);

			/* Sets the scale.
			 * @newScale A <Vector3> describing the new scale.
			 */
			void setScale(Math::Vector3 newScale);

			//Override so that this <Component> can be copied across <GameObject>s
			virtual Component* Clone() override;

			//Override for Update Logic
			virtual void Update();

			//Override for Render Logic
			virtual void Render();

			//Destructor
			~Transform();

		private:
			Math::Vector3 position;
			Math::Quaternion rotation;
			Math::Vector3 eulerRotation;
			Math::Vector3 scale;

			Math::Vector3 forward;
			Math::Vector3 right;
			Math::Vector3 up;

			Math::Matrix4 modelMatrix;
		};
	}
}
#endif