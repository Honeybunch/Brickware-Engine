#ifndef MESH_H
#define MESH_H

#include "BrickwareCoreDLL.h"

#include "Settings.h"

#define _CRT_SECURE_NO_WARNINGS 

#ifdef D3D_SUPPORT
#define NOMINMAX //Without this windows.h will break std::limits
#include <windows.h>
#endif

#ifdef GL_SUPPORT
#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <limits>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

#include "Transform.h"
#include "Shader.h"
#include "GameObject.h"
#include "Bounds.h"

class BRICKWARE_CORE_API Mesh
{
public :
	Mesh(char* modelFilePath);

	float* getPoints();
	Bounds getBounds();

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

	Bounds bounds;

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

#endif