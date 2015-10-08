#ifdef GL_SUPPORT

#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/PrimitiveManager.hpp"
#include "BrickwareGraphics/Primitive.hpp"

using namespace Brickware;
using namespace Graphics;

//Initialization of static variables

GLuint PrimitiveManager::pointVAO;
GLuint PrimitiveManager::pointVBO;
GLuint PrimitiveManager::pointIBO;

GLuint PrimitiveManager::lineVAO;
GLuint PrimitiveManager::lineVBO;
GLuint PrimitiveManager::lineIBO;

GLuint PrimitiveManager::lineQuadVAO;
GLuint PrimitiveManager::lineQuadVBO;
GLuint PrimitiveManager::lineQuadIBO;

GLuint PrimitiveManager::lineCircleVAO;
GLuint PrimitiveManager::lineCircleVBO;
GLuint PrimitiveManager::lineCircleIBO;

GLuint PrimitiveManager::fillQuadVAO;
GLuint PrimitiveManager::fillQuadVBO;
GLuint PrimitiveManager::fillQuadIBO;

GLuint PrimitiveManager::fillCircleVAO;
GLuint PrimitiveManager::fillCircleVBO;
GLuint PrimitiveManager::fillCircleIBO;

void PrimitiveManager::BufferDataGL()
{
	//Data that we're going to buffer
	float point[3] = { 0, 0, 0 };
	unsigned short pointIndices[1] = { 0 };

	float line[6] = { 0, 0, 0,
		1.0f, 0, 0 };
	unsigned short lineIndices[2] = { 0, 1 };

	float lineQuad[12] = { -0.5f, 0.5f, 0,
		0.5f, 0.5f, 0,
		0.5f, -0.5f, 0,
		-0.5f, -0.5f, 0 };
	unsigned short lineQuadIndices[4] = { 0, 1, 2, 3 };

	//Generate points for line circle
	const int circlePoints = 20;
	float lineCircle[circlePoints * 3];
	unsigned short lineCircleIndices[circlePoints];

	float radsPerPoint = (float)(M_PI_2 / circlePoints);

	//We have to calculate the points in a circle
	for (int i = 0; i < 20; i++)
	{
		float x = cosf(radsPerPoint * i);
		float y = sinf(radsPerPoint * i);

		int index = (i * 3);

		lineCircle[index] = x;
		lineCircle[index + 1] = y;
		lineCircle[index + 2] = 0;

		lineCircleIndices[i] = i;
	}

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

	//Buffer line quad
	glGenVertexArrays(1, &lineQuadVAO);
	glBindVertexArray(lineQuadVAO);

	glGenBuffers(1, &lineQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), lineQuad, GL_STATIC_DRAW);

	glGenBuffers(1, &lineQuadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineQuadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned short), lineQuadIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//Buffer line circle
	glGenVertexArrays(1, &lineCircleVAO);
	glBindVertexArray(lineCircleVAO);

	glGenBuffers(1, &lineCircleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineCircleVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), lineCircle, GL_STATIC_DRAW);

	glGenBuffers(1, &lineCircleIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineCircleIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(unsigned short), lineCircleIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//Unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PrimitiveManager::DrawPrimitiveGL(Primitive* p)
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

	SetColorGL(p->color);
	SetPointSizeGL(p->pointSize);
	SetLineWidthGL(p->lineWidth);

	GLuint worldLocation = (GLuint)(shader->uniformMap["worldMatrix"]);
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, p->worldMatrix.getAsArray());

	//Setup primitive VAO
	glBindVertexArray(p->vao);
	glBindAttribLocation(shader->shaderProgram, 0, "position");
	
	//Bind ibo and draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->ibo);
	glDrawElements(drawType, p->pointCount, GL_UNSIGNED_SHORT, (void*)0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void PrimitiveManager::SetColorGL(Math::Vector4 color)
{
	GLuint colorLocation = (GLuint)(shader->uniformMap["color"]);
	glUniform4fv(colorLocation, 1, color.getAsArray());
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

	glDeleteVertexArrays(1, &lineQuadVAO);
	glDeleteBuffers(1, &lineQuadVBO);
	glDeleteBuffers(1, &lineQuadIBO);

	glDeleteVertexArrays(1, &lineCircleVAO);
	glDeleteBuffers(1, &lineCircleVBO);
	glDeleteBuffers(1, &lineCircleIBO);

	glDeleteVertexArrays(1, &fillQuadVAO);
	glDeleteBuffers(1, &fillQuadVBO);
	glDeleteBuffers(1, &fillQuadIBO);

	glDeleteVertexArrays(1, &fillCircleVAO);
	glDeleteBuffers(1, &fillCircleVBO);
	glDeleteBuffers(1, &fillCircleIBO);
}

#endif