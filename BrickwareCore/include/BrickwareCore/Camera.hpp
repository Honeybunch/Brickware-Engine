#ifndef CAMERA_H
#define CAMERA_H

//Pre-Include Defines
#define _USE_MATH_DEFINES

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//Graphics Headers
#ifdef D3D_SUPPORT
#include <windows.h>
#endif

#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "BrickwareMath/Matrix4.hpp"
#include "BrickwareGraphics/Material.hpp"

//System Level Headers
#include <iostream>
#include <vector>
#include <math.h>

//Project Headers
#include "BrickwareCore/Settings.hpp"
#include "BrickwareCore/Transform.hpp"
#include "BrickwareCore/GameObject.hpp"

namespace Brickware
{
	namespace Core
	{
		/// Used to avoid warnings about exporting std::vectors
		class Camera;

#ifdef _WIN32
		template class BRICKWARE_CORE_API std::vector < Camera* > ;
#endif
		/* A Special <Component> that should be added to <GameObject>s that you want to act like Cameras
		 * 
		 * Only the Camera marked as active will be the one rendering to the back buffer or 
		 * post processing. Also noteworthy is the fact that the Camera will send data directly
		 * to materials but the view and projection data will be sent to the 
		 * <Brickware::Graphics::PrimitiveManager> in the <Game> class. This 
		 * behaviour is subject to change.
		 *
		 * Also this Camera currently works as a first person character controller.
		 * This behaviour will be removed in the future and moved int a character controller
		 * class.
		 */
		class BRICKWARE_CORE_API Camera : public Component
		{
		public:
			/* Gets the current active Camera 
			 * @return A pointer to the current active Camera.
			 */
			static Camera* GetActiveCamera();

			/* Constructor
			 * @FoV Field of View
			 * @width Viewport width
			 * @height Viewport Height
			 * @zNear Near plane depth
			 * @zFar Far plane depth
			 */
			Camera(float FoV, float width, float height, float zNear, float zFar);

			/* Gets the look direction
			 * @return A <Math::Vector3> that represents this camera's look direction.
			 */
			Math::Vector3 getLookAt();

			/* Gets the view matrix
			 * @return A <Math::Matrix4> that represents this camera's view matrix
			 */
			Math::Matrix4 getViewMatrix();
			
			/* Gets the projection matrix
			 * @return a <Math::Matrix4> that represents this camera's projection matrix
			 */
			Math::Matrix4 getProjectionMatrix();

			/* Set the camera's look at direction
			 * @lookAt The direction that you want the camera to look at
			 */
			void setLookAt(Math::Vector3 lookAt);
			
			/* Set this Camera to be the active camera.
			 * This will make sure that all other cameras are inactive.
			 */
			void setActive();
			
			//Moves the Camera's <GameObject> forward
			void moveForward();
			//Moves the Camera's <GameObject> backward
			void moveBackward();
			//Strafes the Camera's <GameObject> left
			void moveLeft();
			//Strafes the Camera's <GameObject> right
			void moveRight();

			//Currently does nothing, should setup a rendering octree
			virtual void Start() override;
			//Handles Camera look at calculation and matrix updating
			virtual void Update() override;

			//Sends data to materials for rendering
			void Render();

			//Destructor
			~Camera(void);

		private:
			static std::vector<Camera*> SceneCameras;
			static Camera* ActiveCamera;

			bool active = false;

			float FoV;
			float width;
			float height;
			float zNear;
			float zFar;

			Math::Matrix4 viewMatrix;
			Math::Matrix4 projectionMatrix;

			float speed;
			Math::Vector3 lookAt;

			Math::Matrix4 calcViewMatrix();
			Math::Matrix4 calcProjectionMatrix();
		};
	}
}
#endif

