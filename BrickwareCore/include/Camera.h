#ifndef CAMERA_H
#define CAMERA_H

//Pre-Include Defines
#define _USE_MATH_DEFINES

//DLL Header
#include "BrickwareCoreDLL.h"

//Graphics Headers
#ifdef D3D_SUPPORT
#include <windows.h>
#endif

#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "Matrix4.h"

//System Level Headers
#include <iostream>
#include <vector>
#include <math.h>

//Project Headers
#include "Settings.h"
#include "Transform.h"
#include "GameObject.h"

namespace Brickware
{
	namespace Core
	{
		//Used to avoid warnings about exporting std::vectors
		class Camera;
		template class BRICKWARE_CORE_API std::vector < Camera* > ;

		class BRICKWARE_CORE_API Camera : public Component
		{
		public:
			static Camera* GetActiveCamera();

			Camera(float FoV, float width, float height, float zNear, float zFar);
			Camera(Transform* transform, float FoV, float width, float height, float zNear, float zFar);
			~Camera(void);

			Math::Vector3 getLookAt();
			Math::Matrix4 getViewMatrix();
			Math::Matrix4 getProjectionMatrix();

			void setLookAt(Math::Vector3 lookAt);

			void setActive();

			void moveForward();
			void moveBackward();
			void moveLeft();
			void moveRight();

			virtual void Start() override;
			virtual void Update() override;
			void Render();

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
	};
};

#endif

