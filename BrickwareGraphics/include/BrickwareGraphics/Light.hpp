#ifndef LIGHT_H
#define LIGHT_H

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "BrickwareMath/Vector3.hpp"

//System Level Headers
#include <string>

//Project Headers
#include "BrickwareGraphics/Material.hpp"

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
			int lightIndex;

			Math::Vector3 position;
			Math::Vector3 ambientColor;
			Math::Vector3 diffuseColor;
			Math::Vector3 specularColor;
		};
	}
}
#endif

