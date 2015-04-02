#ifndef LIGHT_H
#define LIGHT_H

//DLL Header
#include "BrickwareGraphicsDLL.h"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "Vector3.h"

//System Level Headers
#include <string>

//Project Headers
#include "Material.h"

namespace Brickware
{
	namespace Graphics
	{
		class BRICKWARE_GRAPHICS_API Light
		{
		public:
			Light();

			static int LightCount;

			void setPosition(Math::Vector3 position);
			void setAmbientColor(Math::Vector3 ambientColor);
			void setDiffuseColor(Math::Vector3 diffuseColor);
			void setSpecularColor(Math::Vector3 specularColor);

			void Render(Material* material);

			~Light(void);

		private:
			Math::Vector3 position;
			Math::Vector3 ambientColor;
			Math::Vector3 diffuseColor;
			Math::Vector3 specularColor;
		};
	}
}
#endif

