#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

//DLL Header
#include "BrickwareCoreDLL.h"

//Other Brickware Project Headers
#include "Vector3.h"

#include "Light.h"
#include "RenderingManager.h"

//Project Headers
#include "Component.h"

namespace Brickware
{
	namespace Core
	{
		class BRICKWARE_CORE_API PointLight : public Component
		{
		public:
			PointLight();

			void setDiffuseColor(Math::Vector3 diffuseColor);
			void setSpecularColor(Math::Vector3 specularColor);

			virtual void Render();
#ifdef _DEBUG
			virtual void DebugDraw();
#endif

			virtual ~PointLight(void);

		private:
			Graphics::Light* light;
		};
	}
}

#endif