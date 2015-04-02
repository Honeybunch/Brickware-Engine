#ifndef PRIMITIVE_MANAGER_H
#define PRIMITIVE_MANAGER_H

//DLL Header
#include "BrickwareGraphicsDLL.h"

//Other Brickware Project Headers
#include "Matrix4.h"

//System Level Headers
#include <vector>

//Project Headers
#include "GraphicsSettings.h"
#include "Shader.h"

namespace Brickware
{
	namespace Graphics
	{
		//Used to avoid warnings about exporting std::vectors
		class Primitive;
		template class BRICKWARE_GRAPHICS_API std::vector < Primitive* > ;

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

#ifdef GL_SUPPORT
			//Primitive buffers
			static GLuint pointVBO;
			static GLuint pointIBO;

			static GLuint lineVBO;
			static GLuint lineIBO;

			static GLuint lineQuadVBO;
			static GLuint lineQuadIBO;

			static GLuint lineCircleVBO;
			static GLuint lineCircleIBO;

			static GLuint fillQuadVBO;
			static GLuint fillQuadIBO;

			static GLuint fillCircleVBO;
			static GLuint fillCircleIBO;

			static void BufferDataGL();

			static void DrawPrimitiveGL(Primitive* p);

			static void SetColorGL(Math::Vector4 color);
			static void SetPointSizeGL(float pointSize);
			static void SetLineWidthGL(float lineWidth);
#endif

#ifdef D3D_SUPPORT
			static void BufferDataD3D();

			static void DrawPrimitiveD3D(Primitive* p, Matrix4 worldMatrix);

			static void SetColorD3D(Math::Vector4 color);
			static void SetPointSizeD3D(float pointSize);
			static void SetLineWidthD3D(float lineWidth);
#endif
		};
	}
}
#endif