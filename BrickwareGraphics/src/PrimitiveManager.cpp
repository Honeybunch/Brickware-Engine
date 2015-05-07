#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/PrimitiveManager.hpp"
#include "BrickwareGraphics/Primitive.hpp"

using namespace Brickware;
using namespace Graphics;

std::vector<Primitive*> PrimitiveManager::Primitives;
Shader* PrimitiveManager::shader;

//Initialization of static variables

#ifdef GL_SUPPORT
GLuint PrimitiveManager::pointVBO;
GLuint PrimitiveManager::pointIBO;

GLuint PrimitiveManager::lineVBO;
GLuint PrimitiveManager::lineIBO;

GLuint PrimitiveManager::lineQuadVBO;
GLuint PrimitiveManager::lineQuadIBO;

GLuint PrimitiveManager::lineCircleVBO;
GLuint PrimitiveManager::lineCircleIBO;

GLuint PrimitiveManager::fillQuadVBO;
GLuint PrimitiveManager::fillQuadIBO;

GLuint PrimitiveManager::fillCircleVBO;
GLuint PrimitiveManager::fillCircleIBO;
#endif
#ifdef D3D_SUPPORT

#endif

//Used to load the basic shader and buffer data
void PrimitiveManager::Initialize()
{
	//We want to create basic buffers of our few basic shapes
#ifdef D3D_SUPPORT
	BufferDataD3D();
#else
	shader = new Shader("Shaders/PrimitiveVertex", "Shaders/PrimitivePixel");
	BufferDataGL();
#endif
}

void PrimitiveManager::ClearPrimitives()
{
	for (unsigned int i = 0; i < Primitives.size(); i++)
		delete Primitives[i];

	Primitives.clear();
}

void PrimitiveManager::DrawPrimitives(Math::Matrix4 viewMatrix, Math::Matrix4 projectionMatrix)
{
	shader->bindShader();

	for (unsigned int i = 0; i < Primitives.size(); i++)
	{
		Primitive* p = Primitives[i];
		p->worldMatrix = (p->modelMatrix * viewMatrix) * projectionMatrix;
#ifdef D3D_SUPPORT
		//DrawPrimitiveD3D(p, modelMatrix, viewMatrix, projectionMatrix);
#else
		DrawPrimitiveGL(p);
#endif
	}

	shader->freeShader();
}

void PrimitiveManager::Destroy()
{
	ClearPrimitives();

	//Delete buffers
#ifdef GL_SUPPORT
	glDeleteBuffers(1, &pointVBO);
	glDeleteBuffers(1, &pointIBO);
#endif

#ifdef D3D_SUPPORT
#endif

	delete shader;
}

#ifdef GL_SUPPORT

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

	float radsPerPoint = (float)(M_2_PI / circlePoints);

	//We have to calculate the points in a circle
	for (int i = 0; i < 20; i++)
	{
		float x = cosf(radsPerPoint * i);
		float y = sinf(radsPerPoint * i);
		
		int index = (i * 3);

		lineCircle[index    ] = x;
		lineCircle[index + 1] = y;
		lineCircle[index + 2] = 0;

		lineCircleIndices[i] = i;
	}

	//Buffer point
	glGenBuffers(1, &pointVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), point, GL_STATIC_DRAW);

	glGenBuffers(1, &pointIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1 * sizeof(unsigned short), pointIndices, GL_STATIC_DRAW);

	//Buffer line
	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), line, GL_STATIC_DRAW);

	glGenBuffers(1, &lineIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(unsigned short), lineIndices, GL_STATIC_DRAW);

	//Buffer line quad
	glGenBuffers(1, &lineQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), lineQuad, GL_STATIC_DRAW);

	glGenBuffers(1, &lineQuadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineQuadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned short), lineQuadIndices, GL_STATIC_DRAW);

	//Buffer line circle
	glGenBuffers(1, &lineCircleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineCircleVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), lineCircle, GL_STATIC_DRAW);

	glGenBuffers(1, &lineCircleIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineCircleIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(unsigned short), lineCircleIndices, GL_STATIC_DRAW);

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

	GLuint worldLocation = (GLuint)(shader->uniformMap[std::string("worldMatrix")]);
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, p->worldMatrix.getAsArray());

	GLuint position = glGetAttribLocation(shader->shaderProgram, "position");
	glEnableVertexAttribArray(position);

	glBindBuffer(GL_ARRAY_BUFFER, p->vbo);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->ibo);

	glDrawElements(drawType, p->pointCount, GL_UNSIGNED_SHORT, (void*)0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void PrimitiveManager::SetColorGL(Math::Vector4 color)
{
	GLuint colorLocation = (GLuint)(shader->uniformMap[std::string("color")]);
	glUniform4fv(colorLocation, 1, color.getAsArray());
}

void PrimitiveManager::SetPointSizeGL(float pointSize)
{
	glPointSize(pointSize);
}

void PrimitiveManager::SetLineWidthGL(float lineWidth)
{
	glLineWidth(lineWidth);
}

#endif

#ifdef D3D_SUPPORT

void PrimitiveManager::BufferDataD3D()
{
	
}

#endif