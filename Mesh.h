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

#include "Component.h"
#include "Transform.h"
#include "Shader.h"
#include "GameObject.h"

class Mesh : public Component
{
public :
	Mesh(GLuint shaderProgram, Shape shape, char* textureFileName = "");
	~Mesh();

	virtual void Render();

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