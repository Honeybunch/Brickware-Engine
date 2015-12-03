#ifndef MESH_H
#define MESH_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

#define NOMINMAX //Without this windows.h will break std::limits

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

#ifdef D3D_SUPPORT
#include <windows.h>
#endif

//Other Brickware Project Headers
#include "BrickwareMath/Vector2.hpp"
#include "BrickwareMath/Vector3.hpp"
#include "BrickwareMath/Bounds.hpp"

//Sytem Level Headers
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>

//Project Headers
#include "BrickwareGraphics/Shader.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

namespace Brickware
{
	namespace Graphics
	{
		enum BRICKWARE_GRAPHICS_API BufferHint
		{
			STATIC_DRAW,
			DYNAMIC_DRAW
		};

		#ifdef _WIN32
		//Warning aversion
		template class __declspec(dllexport) std::vector < Math::Vector3 >;
		template class __declspec(dllexport) std::vector < Math::Vector2 >;
		#endif

		class BRICKWARE_GRAPHICS_API Mesh
		{
		public:
			Mesh();
			Mesh(std::vector<Math::Vector3> verticies, 
				 std::vector<Math::Vector3> normals,
				 std::vector<Math::Vector2> texCoords,
				 std::vector<Math::Vector3> indices);
			Mesh(const char* modelFilePath);

			std::vector<Math::Vector3> getVerticies();
			std::vector<Math::Vector3> getNormals();
			std::vector<Math::Vector2> getTexCoords();
			std::vector<Math::Vector3> getIndices();

			Math::Bounds getBounds();

			size_t getPointSize();
			size_t getNormalSize();
			size_t getIndexSize();
			size_t getNumberOfVerts();
			size_t getTexCoordSize();

			void setBufferHint(BufferHint hint);

			void setVertices(std::vector<Math::Vector3> newVerts);
			void setNormals(std::vector<Math::Vector3> newNormals);
			void setTexCoords(std::vector<Math::Vector2> newTexCoords);
			void setIndices(std::vector<Math::Vector3> newIndices);

			void setBounds(Math::Bounds newBounds);

			//To be called after you've set all the verts, normals, etc
			void bufferChanges();

#ifdef GL_SUPPORT
			GLuint getVAO();
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
			void init();

			void(Mesh::*setBufferHintPtr)(BufferHint hint);
			void(Mesh::*bufferMeshPtr)();
			void(Mesh::*cleanupMeshPtr)();

			void loadOBJ(const char* fileName);

			std::vector<Math::Vector3> modelVerts;
			std::vector<Math::Vector3> modelNormals;
			std::vector<Math::Vector2> modelTexCoords;
			std::vector<Math::Vector3> modelIndices;

			float* points;
			float* normals;
			float* texCoords;

			unsigned short* indices;

			Math::Bounds bounds;

			size_t numberOfVerts;

			size_t pointSize;
			size_t normalSize;
			size_t texCoordSize;

			size_t indexSize;

#ifdef GL_SUPPORT
			//Buffer IDs
			GLuint vao;
			GLuint vbo;
			GLuint ibo;

			int glBufferHint;
			void setBufferHintGL(BufferHint hint);
			void bufferGL();
			void cleanupGL();
#endif

#ifdef D3D_SUPPORT
			ID3D11Buffer* positionBuffer;
			ID3D11Buffer* normalBuffer;
			ID3D11Buffer* texCoordBuffer;

			ID3D11Buffer* indexBuffer;

			void setBufferHintD3D(BufferHint hint);
			void bufferD3D();
			void cleanupD3D();
#endif
		};
	}
}
#endif
