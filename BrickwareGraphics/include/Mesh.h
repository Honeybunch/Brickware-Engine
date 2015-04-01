#ifndef MESH_H
#define MESH_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS 

//DLL Header
#include "BrickwareGraphicsDLL.h"

//Graphics Headers
#ifdef D3D_SUPPORT
#define NOMINMAX //Without this windows.h will break std::limits
#include <windows.h>
#endif

#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

//Other Brickware Project Headers
#include "Vector2.h"
#include "Vector3.h"
#include "Bounds.h"

//Sytem Level Headers
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>

//Project Headers
#include "Shader.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Brickware
{
	namespace Graphics
	{
		class BRICKWARE_GRAPHICS_API Mesh
		{
		public:
			Mesh(char* modelFilePath);

			float* getPoints();
			Math::Bounds getBounds();

			int getPointSize();
			int getNormalSize();
			int getIndexSize();
			int getNumberOfVerts();
			int getTexCoordSize();

#ifdef GL_SUPPORT
			GLuint getVBO();
			GLuint getIBO();
#endif

#ifdef D3D_SUPPORT
			ID3D11Buffer* getPositionBuffer();
			ID3D11Buffer* getNormalBuffer();
			ID3D11Buffer* getTexCoordBuffer();

			ID3D11Buffer* getIndexBuffer();
#endif

			~Mesh();

		private:
			void loadOBJ(char* fileName);

			float* points;
			float* normals;
			unsigned short* indices;
			float* texCoords;

			Math::Bounds bounds;

			int pointSize;
			int normalSize;
			int indexSize;
			int numberOfVerts;
			int texCoordSize;

#ifdef GL_SUPPORT

			//Buffer IDs
			GLuint vbo;
			GLuint ibo;

			void bufferGL();
#endif

#ifdef D3D_SUPPORT
			ID3D11Buffer* positionBuffer;
			ID3D11Buffer* normalBuffer;
			ID3D11Buffer* texCoordBuffer;

			ID3D11Buffer* indexBuffer;

			void bufferD3D();
#endif
		};
	};
};
#endif