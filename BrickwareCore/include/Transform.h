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

		class BRICKWARE_CORE_API Transform : public Component
		{
		public:
			Transform();

			//Accessors
			Math::Vector3 getPosition();
			Math::Vector3 getEulerRotation();
			Math::Quaternion getRotation();
			Math::Vector3 getScale();

			Math::Vector3 getForward();
			Math::Vector3 getRight();
			Math::Vector3 getUp();

			Math::Matrix4 getModelMatrix();

			//Mutators
			void setPosition(Math::Vector3 newPosition);
			void setEulerRotation(Math::Vector3 newRotation);
			void setRotation(Math::Quaternion newRotation);
			void setScale(Math::Vector3 newScale);

			//Component Overrides
			virtual Component* Clone() override;
			virtual void Update();
			virtual void Render();

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