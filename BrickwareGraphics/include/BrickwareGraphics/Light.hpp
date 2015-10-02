#ifndef LIGHT_H
#define LIGHT_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

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
#include "BrickwareGraphics/RendererInfo.hpp"

namespace Brickware
{
	namespace Graphics
	{
		class BRICKWARE_GRAPHICS_API Light
		{
		public:
			Light();

			void setAmbientColor(Math::Vector3 ambientColor);
			void setDiffuseColor(Math::Vector3 diffuseColor);
			void setSpecularColor(Math::Vector3 specularColor);

			void setShadowStrength(float shadowStrength);
			void setShadowBias(float shadowBias);

			void setShadowMapQuality(ShadowQuality shadowQuality);

			virtual void RenderLight(Shader* shader) = 0;
			virtual void RenderShadowMap(Shader* shadowShader) = 0;
			virtual void BindShadowMap(Shader* shader) = 0;

			virtual ~Light(void);

		protected:

			virtual void Init() = 0;

			Math::Matrix4 depthVP;
			Math::Matrix4 depthBiasMVP;

			Math::Vector3 ambientColor;
			Math::Vector3 diffuseColor;
			Math::Vector3 specularColor;

			GLuint shadowBuffer;
			GLuint depthTexture;

			float shadowStrength;
			float shadowBias;
			float shadowNormalBias; //TODO: Implement

			unsigned int shadowMapRes;
		};
	}
}
#endif

