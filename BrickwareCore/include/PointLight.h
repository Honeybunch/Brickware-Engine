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
		/* A <Component> which makes the attached <GameObject> emit light in all directions
		 * 
		 * This <Component> works by sending data about itself and its <GameObject>
		 * to the <Brickware::Graphics::RenderingManager> via <Brickware::Graphics::RederingManager::AddLight>.
		 * Right now there is a hardware limit of 8 lights so any more than that 
		 * may cause undefined behaviour. 
		 */
		class BRICKWARE_CORE_API PointLight : public Component
		{
		public:
			//Blank Constructor
			PointLight();

			/* Sets the diffuse color of the light
			 * @diffuseColor A <Math::Vector3> describing a color's RGB values
			 */
			void setDiffuseColor(Math::Vector3 diffuseColor);

			/* Sets the color of the light's specular highlights
			 * @diffuseColor A <Math::Vector3> describing a color's RGB values
			 */
			void setSpecularColor(Math::Vector3 specularColor);

			//When called will send light data to the <Brickware::Graphics::RenderingManager>
			virtual void Render();
#ifdef _DEBUG
			//When called in Debug mode will draw debug output of the light's position
			virtual void DebugDraw();
#endif

			//Destructor
			virtual ~PointLight(void);

		private:
			Graphics::Light* light;
		};
	}
}

#endif