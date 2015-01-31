/*
	Defines how a piece of geometry is loaded to and drawn on the GPU
 */

#ifndef MESH_H
#define MESH_H

#include <SOIL.h>

#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>

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
	Mesh(GLuint shaderProgram, Shape shape, char* textureFileName = "");

	float* getPoints();

	int getPointSize();
	int getNormalSize();
	int getIndexSize();
	int getNumberOfVerts();
	int getTexCoordSize();

	GLuint getTexture();

	GLuint getVBO();
	GLuint getIBO();

	~Mesh();

private: 
	float* points;
	float* normals;
	GLushort* indicies;
	float* texCoords;

	int pointSize;
	int normalSize;
	int indexSize;
	int numberOfVerts;
	int texCoordSize;

	//Texture info
	GLuint texture;

	//Buffer IDs
	GLuint vbo; //Vertex Buffer Object
	GLuint ibo; //Index Buffer Object

	void bufferGL(GLint shaderProgram, char* textureFileName);
	void bufferD3D();
};

#endif