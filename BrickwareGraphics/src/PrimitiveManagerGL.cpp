#ifdef GL_SUPPORT

#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/PrimitiveManager.hpp"
#include "BrickwareGraphics/Primitive.hpp"
#include "BrickwareGraphics/Screen.hpp"

using namespace Brickware;
using namespace Graphics;

//Initialization of static variables

GLuint PrimitiveManager::pointVAO;
GLuint PrimitiveManager::pointVBO;
GLuint PrimitiveManager::pointIBO;

GLuint PrimitiveManager::lineVAO;
GLuint PrimitiveManager::lineVBO;
GLuint PrimitiveManager::lineIBO;

void PrimitiveManager::BufferDataGL()
{
	//Data that we're going to buffer
	float point[3] = { 0, 0, 0 };
	unsigned short pointIndices[1] = { 0 };

	float line[6] = { 0, 0, 0,
		1.0f, 0, 0 };
	unsigned short lineIndices[2] = { 0, 1 };

	//Buffer point
	glGenVertexArrays(1, &pointVAO);
	glBindVertexArray(pointVAO);

	glGenBuffers(1, &pointVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), point, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glGenBuffers(1, &pointIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1 * sizeof(unsigned short), pointIndices, GL_STATIC_DRAW);

	//Buffer line
	glGenVertexArrays(1, &lineVAO);
	glBindVertexArray(lineVAO);

	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), line, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glGenBuffers(1, &lineIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(unsigned short), lineIndices, GL_STATIC_DRAW);

	//Unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PrimitiveManager::DrawPrimitiveGL(Primitive* p, Shader* shader)
{
	GLenum drawType;

	switch (p->drawType)
	{
	case PrimitiveType::P_POINT:
		drawType = GL_POINTS;
		break;
	case PrimitiveType::P_LINE:
		drawType = GL_LINES;
		break;
	case PrimitiveType::P_FILL:
		drawType = GL_FILL;
		break;
	}

	if (drawType == PrimitiveType::P_POINT){
		GLuint sizeLocation = (GLuint)(shader->uniformMap["size"]);
		glUniform1f(sizeLocation, p->pointSize);

		GLuint ratioLocation = (GLuint)(shader->uniformMap["aspectRatio"]);
		glUniform1f(ratioLocation, Screen::GetAspectRatio());
	}

	//Set Color
	GLuint colorLocation = (GLuint)(shader->uniformMap["color"]);
	glUniform4fv(colorLocation, 1, p->color.getAsArray());

	GLuint worldLocation = (GLuint)(shader->uniformMap["worldMatrix"]);
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, p->worldMatrix.getAsArray());

	//Setup primitive VAO
	glBindVertexArray(p->vao);
	glDrawArrays(drawType, 0, p->pointCount);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void PrimitiveManager::SetPointSizeGL(float pointSize)
{
	glPointSize(pointSize);
}

void PrimitiveManager::SetLineWidthGL(float lineWidth)
{
	//According to GL 3.2 lineWidth can't be larger than 1.0 or <= 0
	if (lineWidth > 1.0f)
		lineWidth = 1.0f;
	else if (lineWidth == 0.0f)
		lineWidth = 0.1f;

	glLineWidth(lineWidth);
}

void PrimitiveManager::DestroyDataGL()
{
	glDeleteVertexArrays(1, &pointVAO);
	glDeleteBuffers(1, &pointVBO);
	glDeleteBuffers(1, &pointIBO);

	glDeleteVertexArrays(1, &lineVAO);
	glDeleteBuffers(1, &lineVBO);
	glDeleteBuffers(1, &lineIBO);
}

#endif