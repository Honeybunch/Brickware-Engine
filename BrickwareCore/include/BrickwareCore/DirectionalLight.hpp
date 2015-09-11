#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

//DLL Header
#include "BrickwareCore/BrickwareCoreDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareMath/Vector3.hpp"

#include "BrickwareGraphics/DirectionalLightInternal.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

//Project Headers
#include "BrickwareCore/Component.hpp"

namespace Brickware
{
	namespace Core
	{
		/* A <Component> which makes the attached <GameObject> emit light in globally in one direction
		*
		* This <Component> works by sending data about itself to the 
		* <Brickware::Graphics::RenderingManager> via <Brickware::Graphics::RederingManager::AddLight>.
		* Only the last defined DirectionalLight will have its data visible in the scene
		*/
		class BRICKWARE_CORE_API DirectionalLight : public Component
		{
		public:
			//Blank Constructor
			DirectionalLight();

			/* Sets the direction that the light will follow
			* @direction A <Math::Vector3> describing the light's direction
			*/
			void setDirection(Math::Vector3 direction);

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
			virtual ~DirectionalLight(void);

		private:
			Graphics::DirectionalLightInternal* light;
		};
	}
}

#endif