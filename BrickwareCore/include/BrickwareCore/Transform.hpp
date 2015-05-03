#ifndef TRANSFORM_H
#define TRANSFORM_H

//DLL Header
#include "BrickwareCore\BrickwareCoreDLL.hpp"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>
#endif

#ifdef D3D_SUPPORT
#include <windows.h>
#endif

//Other Brickware Project Headers
#include "BrickwareMath\Vector3.hpp"
#include "BrickwareMath\Vector4.hpp"
#include "BrickwareMath\Matrix4.hpp"
#include "BrickwareMath\Matrix3.hpp"
#include "BrickwareMath\Quaternion.hpp"

#include "BrickwareGraphics\Shader.hpp"

//Project Headers
#include "BrickwareCore\Settings.hpp"
#include "BrickwareCore\Component.hpp"

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

			/* Gets local rotation in Euler coordinates.
			 * @return A <Vector3> describing local euler rotation.
			 */
			Math::Vector3 getEulerRotation();

			/* Gets local quaternion rotation.
			 * @return A <Quaternion> describing local rotation.
			 */
			Math::Quaternion getRotation();

			/* Gets world rotation in Euler coordinates.
			* @return A <Vector3> describing world space euler rotation.
			*/
			//Math::Vector3 getWorldEulerRotation();

			/* Gets world space quaternion rotation.
			* @return A <Quaternion> describing world space rotation.
			*/
			//Math::Quaternion getWorldRotation();
			
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

			/* Gets matrix describing this object's current translation
			 * @return A <Matrix4> describing translation
			 */
			Math::Matrix4 getTranslationMatrix();

			/* Gets matrix describing this object's current rotation
			 * @return A <Matrix4> describing rotation
			 */
			Math::Matrix4 getRotationMatrix();

			/* Gets matrix describing this object's current scale
			 * @return A <Matrix4> describing scale
			 */
			Math::Matrix4 getScaleMatrix();

			/* Sets the position.
			 * @newPosition A <Vector3> describing the new position.
			 */
			void setPosition(Math::Vector3 newPosition);

			/* Sets the local rotation in Euler coordinates.
			 * @newRotation A <Vector3> describing the new local Euler rotation.
			 * 
			 * This WILL update the local rotation <Quaterion> to match.
			 */
			void setEulerRotation(Math::Vector3 newRotation);

			/* Sets the local rotation.
			 * @newRotation A <Quaternion> describing the new local rotation.
			 *
			 * This will NOT update the local euler rotation vector to match.
			 */
			void setRotation(Math::Quaternion newRotation);

			/* Sets the world rotation in Euler coordinates.
			* @newRotation A <Vector3> describing the new world Euler rotation.
			*
			* This WILL update the world rotation <Quaterion> to match.
			*/
			//void setWorldEulerRotation(Math::Vector3 newRotation);

			/* Sets the world rotation.
			* @newRotation A <Quaternion> describing the new world rotation.
			*
			* This will NOT update the world euler rotation vector to match.
			*/
			//void setWorldRotation(Math::Quaternion newRotation);

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
			Math::Quaternion worldRotation;

			Math::Vector3 eulerRotation;
			Math::Vector3 worldEulerRotation;
			
			Math::Vector3 scale;

			Math::Vector3 forward;
			Math::Vector3 right;
			Math::Vector3 up;

			Math::Matrix4 modelMatrix;

			Math::Matrix4 translationMatrix;
			Math::Matrix4 rotationMatrix;
			Math::Matrix4 scaleMatrix;
		};
	}
}
#endif