/*
	Defines how a piece of geometry is loaded to and drawn on the GPU
 */

#ifndef MESH_H
#define MESH_H

#include <SOIL.h>

#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
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
	Mesh(GLuint shaderProgram, Shape shape, char* textureFileName = "");

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
};

#endif