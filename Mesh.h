/*
	Defines how a piece of geometry is loaded to and drawn on the GPU
 */

#ifndef MESH_H
#define MESH_H

#include "Settings.h"

#ifdef D3D_SUPPORT
	#include <windows.h>
#endif

#ifndef USE_D3D_ONLY
	#include <SOIL.h>

	#include <GL/glew.h>

	#ifdef _WIN32
		#define GLFW_DLL
	#endif

	#define GLFW_INCLUDE_GLU
	#include <glfw3.h>
#endif

#include <vector>
#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

#include "Shape.h"
#include "Transform.h"
#include "Shader.h"
#include "GameObject.h"

class Mesh
{
public :
	Mesh(Shader* shader, Shape shape, char* textureFileName = "");

	float* getPoints();

	int getPointSize();
	int getNormalSize();
	int getIndexSize();
	int getNumberOfVerts();
	int getTexCoordSize();

#ifndef USE_D3D_ONLY
	GLuint getTexture();

	GLuint getVBO();
	GLuint getIBO();
#endif

#ifdef D3D_SUPPORT
	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
#endif

	~Mesh();

private: 
	float* points;
	float* normals;
	unsigned short* indicies;
	float* texCoords;

	int pointSize;
	int normalSize;
	int indexSize;
	int numberOfVerts;
	int texCoordSize;

#ifndef USE_D3D_ONLY
	//Texture info
	GLuint texture;

	//Buffer IDs
	GLuint vbo;
	GLuint ibo; 

	void bufferGL(Shader* shader, char* textureFileName);

#endif

#ifdef D3D_SUPPORT
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	void bufferD3D(Shader* shader, char* textureFileName);
#endif
};

#endif