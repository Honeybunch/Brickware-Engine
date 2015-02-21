#define BRICKWARE_CORE_EXPORTS

#include "Primitive.h"

Vector4 Primitive::currentColor;
float Primitive::currentPointSize;
float Primitive::currentLineWidth;

void Primitive::SetColor(Vector4 color){ currentColor = color; }
void Primitive::SetPointSize(float pointSize){ currentPointSize = pointSize; }
void Primitive::SetLineWidth(float lineWidth){ currentLineWidth = lineWidth; }

void Primitive::DrawPoint(Vector3 point)
{
	std::vector<Vector3> points;
	points.push_back(point);
	Primitive* p = new Primitive(points, PrimitiveType::POINT);
}
void Primitive::DrawLine(Vector3 start, Vector3 end)
{
	std::vector<Vector3> points;

	points.push_back(start);
	points.push_back(end);

	Primitive* p = new Primitive(points, PrimitiveType::LINE);
}
void Primitive::DrawQuad(Vector3 topLeft, Vector3 topRight, Vector3 bottomRight, Vector3 bottomLeft, Vector3 rotation)
{
	std::vector<Vector3> points;

	//Need to add points in a counter clockwise fashion
	points.push_back(topLeft);
	points.push_back(bottomLeft);
	points.push_back(bottomRight);
	points.push_back(topRight);

	Primitive* p = new Primitive(points, PrimitiveType::LINE);
	p->rotation = Quaternion(rotation);
}
void Primitive::DrawCircle(Vector3 center, float radius, int pointCount, Vector3 rotation)
{
	std::vector<Vector3> points;

	//if we're given 4 or less points that's going to be boring so there is a minimum of 5 points
	if (pointCount < 5)
		pointCount = 5;

	float radsPerPoint = (float)(M_2_PI / pointCount);

	//We have to calculate the points in a circle
	for (int i = 0; i < pointCount; i++)
	{
		float x = cosf(radsPerPoint * i) * radius;
		float y = sinf(radsPerPoint * i) * radius;
		points.push_back(Vector3(x,y,0));
	}

	Primitive* p = new Primitive(points, PrimitiveType::LINE);
	p->rotation = Quaternion(rotation);
}

void Primitive::FillQuad(Vector3 topLeft, Vector3 topRight, Vector3 bottomRight, Vector3 bottomLeft, Vector3 rotation){}
void Primitive::FillCircle(Vector3 center, float radius, int pointCount, Vector3 rotation){}

Primitive::Primitive(std::vector<Vector3> points, PrimitiveType drawType)
{
	//We need to get this point data into something we can send to OpenGL or DirectX
	elementCount = points.size() * 3;
	pointCount = points.size();

	pointElements = new float[elementCount];
	pointIndices = new unsigned short[pointCount];
	for (unsigned int i = 0; i < points.size(); i+=3)
	{
		pointElements[i]	= points[i][0];
		pointElements[i+1]	= points[i][1];
		pointElements[i+2]	= points[i][2];

		pointIndices[i] = i;
	}

	this->color = currentColor;
	this->pointSize = currentPointSize;
	this->lineWidth = currentLineWidth;

	this->drawType = drawType;

	createBuffers();

	PrimitiveManager::Primitives.push_back(this);
}

void Primitive::createBuffers()
{
#ifdef D3D_SUPPORT
	createBuffersD3D();
#else
	createBuffersGL();
#endif
}

void Primitive::freeBuffers()
{
#ifdef D3D_SUPPORT
	freeBuffersD3D();
#else
	freeBuffersGL();
#endif
}

#ifdef GL_SUPPORT

void Primitive::createBuffersGL()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, elementCount * sizeof(float), pointElements, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointCount * sizeof(unsigned int), pointIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Primitive::freeBuffersGL()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

#endif

#ifdef D3D_SUPPORT

void Primitive::createBuffersD3D()
{

}
void Primitive::freeBuffersD3D()
{
	
}

#endif


Primitive::~Primitive()
{
	delete[] pointElements;
	delete[] pointIndices;

	freeBuffers();
}