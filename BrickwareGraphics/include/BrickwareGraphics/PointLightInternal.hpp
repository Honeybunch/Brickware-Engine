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
		using namespace Math;

#ifdef GL_SUPPORT
		struct CameraDirection
		{
			GLenum CubemapFace;
			Vector3 Target;
			Vector3 Up;
		};
#endif

		class BRICKWARE_GRAPHICS_API PointLightInternal : public Light
		{
		public:
			PointLightInternal();

			void setPosition(Math::Vector3 position);

			void setConstant(float constant);
			void setLinear(float linear);
			void setQuadratic(float quadratic);

			virtual void RenderLight(Shader* shader);
			virtual void RenderShadowMap();
			virtual void BindShadowMap(Shader* shader);

			virtual ~PointLightInternal(void);

		protected:
			virtual void Init();

			static int LightCount;
			int lightIndex;
			float farPlane;

			Vector3 position;

			float constant;
			float linear;
			float quadratic;

			void (PointLightInternal::*InitPtr)();
			void (PointLightInternal::*RenderShadowMapPtr)();
			void (PointLightInternal::*BindShadowMapPtr)(Shader* shader);

#ifdef GL_SUPPORT
			static CameraDirection CameraDirections[6];

			void InitGL();
			void RenderShadowMapGL();
			void BindShadowMapGL(Shader* shader);
#endif
#ifdef D3D_SUPPORT
			void InitD3D();
			void RenderShadowMapD3D();
			void BindShadowMapD3D(Shader* shader);
#endif
		};
	}
}
#endif

