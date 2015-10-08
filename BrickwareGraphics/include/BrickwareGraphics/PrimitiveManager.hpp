#ifndef PRIMITIVE_MANAGER_H
#define PRIMITIVE_MANAGER_H

//Pre-Include Defines
#define _USE_MATH_DEFINES

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Other Brickware Project Headers
#include "BrickwareMath/Matrix4.hpp"

//System Level Headers
#include <vector>
#include <cmath>

//Project Headers
#include "BrickwareGraphics/GraphicsSettings.hpp"
#include "BrickwareGraphics/Shader.hpp"

namespace Brickware
{
	namespace Graphics
	{
		//Used to avoid warnings about exporting std::vectors
		class Primitive;
#ifdef _WIN32
		template class BRICKWARE_GRAPHICS_API std::vector < Primitive* > ;
#endif

		/*
			This is effectively a singleton class to the Primitive class however we only want these
			methods to be exposed to the Game class.
			*/

		class BRICKWARE_GRAPHICS_API PrimitiveManager
		{
			friend class Primitive;

		public:
			static void Initialize();
			static void ClearPrimitives();
			static void DrawPrimitives(Math::Matrix4 viewMatrix, Math::Matrix4 projectionMatrix);
			static void Destroy();

		private:
			static std::vector<Primitive*> Primitives;
			static Shader* shader;

			static void (*DrawPrimitivePtr)(Primitive* p);
			static void (*BufferDataPtr)();
			static void (*DestroyDataPtr)();

#ifdef GL_SUPPORT
			//Primitive buffers
			static GLuint pointVAO;
			static GLuint pointVBO;
			static GLuint pointIBO;

			static GLuint lineVAO;
			static GLuint lineVBO;
			static GLuint lineIBO;

			static GLuint lineQuadVAO;
			static GLuint lineQuadVBO;
			static GLuint lineQuadIBO;

			static GLuint lineCircleVAO;
			static GLuint lineCircleVBO;
			static GLuint lineCircleIBO;

			static GLuint fillQuadVAO;
			static GLuint fillQuadVBO;
			static GLuint fillQuadIBO;

			static GLuint fillCircleVAO;
			static GLuint fillCircleVBO;
			static GLuint fillCircleIBO;

			static void BufferDataGL();
			static void DestroyDataGL();

			static void DrawPrimitiveGL(Primitive* p);

			static void SetColorGL(Math::Vector4 color);
			static void SetPointSizeGL(float pointSize);
			static void SetLineWidthGL(float lineWidth);
#endif

#ifdef D3D_SUPPORT
			static void BufferDataD3D();
			static void DestroyDataD3D();

			static void DrawPrimitiveD3D(Primitive* p);

			static void SetColorD3D(Math::Vector4 color);
			static void SetPointSizeD3D(float pointSize);
			static void SetLineWidthD3D(float lineWidth);
#endif
		};
	}
}
#endif
