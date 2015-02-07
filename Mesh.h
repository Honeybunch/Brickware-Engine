/*
	Defines how a piece of geometry is loaded to and drawn on the GPU
 */

#ifndef MESH_H
#define MESH_H

#include "Settings.h"

#ifdef D3D_SUPPORT
#include <windows.h>
#endif

#ifdef GL_SUPPORT
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
	Mesh(Shape shape, char* textureFileName = "");

	float* getPoints();

	int getPointSize();
	int getNormalSize();
	int getIndexSize();
	int getNumberOfVerts();
	int getTexCoordSize();

#ifdef GL_SUPPORT
	GLuint getTexture();

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
	float* points;
	float* normals;
	unsigned short* indicies;
	float* texCoords;

	int pointSize;
	int normalSize;
	int indexSize;
	int numberOfVerts;
	int texCoordSize;

#ifdef GL_SUPPORT
	//Texture info
	GLuint texture;

	//Buffer IDs
	GLuint vbo;
	GLuint ibo; 

	void bufferGL(char* textureFileName);
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