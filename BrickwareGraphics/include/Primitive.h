#ifndef PRIMITIVE_H
#define PRIMITIVE_H

//Pre-Include Headers
#define _USE_MATH_DEFINES

//DLL Header
#include "BrickwareGraphicsDLL.h"

//Other Brickware Project Headers
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

//System Level Headers
#include <vector>
#include <cmath>

//Project Headers
#include "PrimitiveManager.h"

namespace Brickware
{
	namespace Graphics
	{
		enum BRICKWARE_GRAPHICS_API PrimitiveType
		{
			P_POINT,
			P_LINE,
			P_FILL
		};

		class BRICKWARE_GRAPHICS_API Primitive
		{
			friend class PrimitiveManager;

		public:
			static void SetColor(Math::Vector4 color);
			static void SetPointSize(float pointSize); //Really only going to work with points
			static void SetLineWidth(float lineWidth);

			static void DrawPoint(Math::Vector3 point);
			static void DrawLine(Math::Vector3 start, Math::Vector3 end);
			static void DrawQuad(Math::Vector3 topLeft, Math::Vector3 topRight, Math::Vector3 bottomRight, Math::Vector3 bottomLeft, Math::Vector3 rotation);
			static void DrawCircle(Math::Vector3 center, float radius, Math::Vector3 rotation);

			static void FillQuad(Math::Vector3 topLeft, Math::Vector3 topRight, Math::Vector3 bottomRight, Math::Vector3 bottomLeft, Math::Vector3 rotation);
			static void FillCircle(Math::Vector3 center, float radius, Math::Vector3 rotation);

		private:
#ifdef GL_SUPPORT
			Primitive(GLuint vbo, GLuint ibo, Math::Vector3 translation, Math::Vector3 scale, Math::Quaternion rotation, PrimitiveType drawType);
#endif

#ifdef D3D_SUPPORT
			Primitive(ID3D11Buffer* vbo, ID3D11Buffer* ibo, Math::Vector3 translation, Math::Vector3 scale, Math::Vector3 rotation, PrimitiveType drawType);
#endif
			~Primitive();

#ifdef GL_SUPPORT
			GLuint vbo;
			GLuint ibo;
#endif
#ifdef D3D_SUPPORT
			ID3D11Buffer* positionBuffer;
			ID3D11Buffer* indexBuffer;
#endif
			static Math::Vector4 currentColor;
			static float currentPointSize;
			static float currentLineWidth;

			unsigned int pointCount; //Number of points in this primitive
			Math::Vector4 color;
			float pointSize;
			float lineWidth;
			PrimitiveType drawType;

			Math::Matrix4 modelMatrix;
			Math::Matrix4 worldMatrix;
		};
	}
}
#endif