#ifndef POINT_LIGHT_INTERNAL_H
#define POINT_LIGHT_INTERNAL_H

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
#include "BrickwareGraphics/Light.hpp"

namespace Brickware
{
	namespace Graphics
	{
		class BRICKWARE_GRAPHICS_API PointLightInternal : public Light
		{
		public:
			PointLightInternal();

			void setPosition(Math::Vector3 position);

			void setConstant(float constant);
			void setLinear(float linear);
			void setQuadratic(float quadratic);

			virtual void RenderLight(Shader* shader);
			virtual void RenderShadowMap(Shader* shadowShader);
			virtual void BindShadowMap(Shader* shader);

			virtual ~PointLightInternal(void);

		protected:
			virtual void Init();

			static int LightCount;
			int lightIndex;

			Math::Vector3 position;

			float constant;
			float linear;
			float quadratic;

			void (PointLightInternal::*InitPtr)();
			void (PointLightInternal::*RenderShadowMapPtr)(Shader* shadowShader);
			void (PointLightInternal::*BindShadowMapPtr)(Shader* shader);

#ifdef GL_SUPPORT
			void InitGL();
			void RenderShadowMapGL(Shader* shadowShader);
			void BindShadowMapGL(Shader* shader);
#endif
#ifdef D3D_SUPPORT
			void InitD3D();
			void RenderShadowMapD3D(Shader* shadowShader);
			void BindShadowMapD3D(Shader* shader);
#endif
		};
	}
}
#endif

