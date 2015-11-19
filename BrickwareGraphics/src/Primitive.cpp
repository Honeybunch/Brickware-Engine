#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Primitive.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

Vector4 Primitive::currentColor;
float Primitive::currentPointSize;
float Primitive::currentLineWidth;

void Primitive::SetColor(Vector4 color){ currentColor = color; }
void Primitive::SetPointSize(float pointSize){ currentPointSize = pointSize; }
void Primitive::SetLineWidth(float lineWidth){ currentLineWidth = lineWidth; }

void Primitive::DrawPoint(Vector3 point)
{
#ifdef GL_SUPPORT
	Primitive* p = new Primitive(PrimitiveManager::pointVAO, PrimitiveManager::pointVBO, PrimitiveManager::pointIBO,
								 point, Vector3(1,1,1), Vector3(), 
								 PrimitiveType::P_POINT);
	p->pointCount = 1;
#endif
}
void Primitive::DrawLine(Vector3 start, Vector3 end)
{
	//Determine distance between points for scale
	Vector3 delta = end - start;
	float sqSum = (delta.getX() * delta.getX()) + (delta.getY() * delta.getY()) + (delta.getZ() * delta.getZ());
	float distance = sqrtf(sqSum);

	Vector3 scale(distance, 1, 1); //The original line buffer is just along x, so that's all we need to scale along

	//Determine angle between points for rotation
	Vector3 rotationAxis = Vector3::Normalize(Vector3::Cross(Vector3(1, 0, 0), Vector3::Normalize(delta)));

	float cosOfAngle = Vector3::Dot(Vector3(1, 0, 0), delta) / (distance);
		
	float angle = acosf(cosOfAngle);
	Vector4 angleAxisRotation = Vector4(rotationAxis, angle);
	Quaternion rotation(angleAxisRotation);

#ifdef GL_SUPPORT
	Primitive* p = new Primitive(PrimitiveManager::lineVAO, PrimitiveManager::lineVBO, PrimitiveManager::lineIBO,
		start, scale, rotation,
		PrimitiveType::P_LINE);
	p->pointCount = 2;
#endif
}

#ifdef GL_SUPPORT

Primitive::Primitive(GLuint vao, GLuint vbo, GLuint ibo, Vector3 translation, Vector3 scale, Quaternion rotation, PrimitiveType drawType)
{
	this->vao = vao;
	this->vbo = vbo;
	this->ibo = ibo;

	this->color = currentColor;
	this->pointSize = currentPointSize;
	this->lineWidth = currentLineWidth;

	this->drawType = drawType;

	//Calculate worldMatrix
	Matrix4 translationMat = Matrix4(1, 0, 0, 0,
								     0, 1, 0, 0,
								     0, 0, 1, 0,
								     translation.getX(),
								     translation.getY(),
								     translation.getZ(),
								     1);
	Matrix4 rotationMat = rotation.getRotationMatrix();

	Matrix4 scaleMat = Matrix4(scale.getX(), 0, 0, 0,
							   0, scale.getY(), 0, 0,
							   0, 0, scale.getZ(), 0,
							   0, 0, 0, 1);

	modelMatrix = scaleMat * rotationMat * translationMat;

	if (drawType == PrimitiveType::P_POINT)
		PrimitiveManager::PointPrimitives.push_back(this);
	else if (drawType == PrimitiveType::P_LINE)
		PrimitiveManager::LinePrimitives.push_back(this);
}
#endif


Primitive::~Primitive()
{
}