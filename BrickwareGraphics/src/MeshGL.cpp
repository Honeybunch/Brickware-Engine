#ifdef GL_SUPPORT

#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Mesh.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;
using namespace Math;

void Mesh::setBufferHintGL(BufferHint hint)
{
	if (hint == BufferHint::STATIC_DRAW)
		glBufferHint = GL_STATIC_DRAW;
	else if (hint == BufferHint::DYNAMIC_DRAW)
		glBufferHint = GL_DYNAMIC_DRAW;
}

GLuint Mesh::getVAO(){ return vao; }
GLuint Mesh::getVBO(){ return vbo; }
GLuint Mesh::getIBO(){ return ibo; }

void Mesh::bufferGL()
{
	//Setup VAO
	glDeleteVertexArrays(1, &vao);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Setup the VBO
	glDeleteBuffers(1, &vbo);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, pointSize + normalSize + texCoordSize, nullptr, glBufferHint);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pointSize, points);
	glBufferSubData(GL_ARRAY_BUFFER, pointSize, normalSize, normals);
	glBufferSubData(GL_ARRAY_BUFFER, pointSize + normalSize, texCoordSize, texCoords);

	//Setup the IBO
	glDeleteBuffers(1, &ibo);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(pointSize));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(pointSize + normalSize));
	glEnableVertexAttribArray(2);

	//Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::cleanupGL()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

#endif