#include "mesh.h"

//Constructor
//Take in the GPU program ID and the vertex data and buffer that data into the VBO and IBO
Mesh::Mesh(GLuint shaderProgram, Shape shape, char* textureFileName)
{
	//Store data from shape
	points = shape.getPoints();
	normals = shape.getNormals();
	indicies = shape.getIndicies();
	texCoords = shape.getTexCoords();

	//Store sizes
	numberOfVerts = shape.getPointCount() / 4;
	pointSize = shape.getPointCount() * sizeof (float);
	normalSize = shape.getNormalCount() * sizeof(float);
	indexSize = numberOfVerts * sizeof(GLushort);
	texCoordSize = shape.getTexCoordSize() * sizeof(float);

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		bufferD3D();
	else
		bufferGL(shaderProgram, textureFileName);
#elif defined(USE_D3D_ONLY)
	bufferD3D();
#else
	bufferGL(shaderProgram, textureFileName);
#endif
}

//Accessors
float* Mesh::getPoints(){ return points; }

int Mesh::getPointSize(){ return pointSize; }
int Mesh::getNormalSize(){ return normalSize; }
int Mesh::getIndexSize(){ return indexSize; }
int Mesh::getNumberOfVerts(){ return numberOfVerts; }
int Mesh::getTexCoordSize(){ return texCoordSize; }

GLuint Mesh::getTexture(){ return texture; }

GLuint Mesh::getVBO(){ return vbo; }
GLuint Mesh::getIBO(){ return ibo; }

//Private functions

void Mesh::bufferGL(GLint shaderProgram, char* textureFileName)
{
	//Use the shader program
	glUseProgram(shaderProgram);

	//Setup the VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pointSize + normalSize + texCoordSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pointSize, points);
	glBufferSubData(GL_ARRAY_BUFFER, pointSize, normalSize, normals);
	glBufferSubData(GL_ARRAY_BUFFER, pointSize + normalSize, texCoordSize, texCoords);

	//Setup the IBO
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indicies, GL_STATIC_DRAW);

	//Load texture with SOIL
	if (textureFileName != "" && textureFileName != NULL)
		texture = SOIL_load_OGL_texture(textureFileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	//Unbind everything
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Mesh::bufferD3D()
{
	//TODO
}

//Destructor
Mesh::~Mesh()
{
	
}

