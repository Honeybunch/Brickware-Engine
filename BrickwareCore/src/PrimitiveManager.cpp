#define BRICKWARE_CORE_EXPORTS

#include "PrimitiveManager.h"
#include "Primitive.h"

std::vector<Primitive*> PrimitiveManager::Primitives;
Shader* PrimitiveManager::shader;

//Used to load the basic shader
void PrimitiveManager::Initialize()
{
	shader = new Shader("Shaders/PrimitiveVertex", "Shaders/PrimitivePixel");
}

void PrimitiveManager::ClearPrimitives()
{
	for (unsigned int i = 0; i < Primitives.size(); i++)
		delete Primitives[i];

	Primitives.clear();
}

void PrimitiveManager::DrawPrimitives()
{
	shader->bindShader();

	Matrix4 viewMatrix = Camera::GetActiveCamera()->getViewMatrix();
	Matrix4 projectionMatrix = Camera::GetActiveCamera()->getProjectionMatrix();

	for (unsigned int i = 0; i < Primitives.size(); i++)
	{
		Primitive* p = Primitives[i];

		Matrix4 rotationMat = p->rotation.getRotationMatrix();
		Matrix4 scaleMat = Matrix4::getIdentityMatrix();
		Matrix4 translationMat = Matrix4::getIdentityMatrix();

		Matrix4 modelMatrix = scaleMat * rotationMat * translationMat;

		Matrix4 worldMatrix = (modelMatrix * viewMatrix) * projectionMatrix;

		Vector4 test = worldMatrix * Vector4(0, 1, 0, 1);

#ifdef D3D_SUPPORT
		//DrawPrimitiveD3D(p, modelMatrix, viewMatrix, projectionMatrix);
#else
		DrawPrimitiveGL(p, modelMatrix, viewMatrix, projectionMatrix);
#endif
	}

	shader->freeShader();
}

void PrimitiveManager::Destroy()
{
	ClearPrimitives();
	delete shader;
}

#ifdef GL_SUPPORT

void PrimitiveManager::DrawPrimitiveGL(Primitive* p, Matrix4 modelMatrix, Matrix4 viewMatrix, Matrix4 projectionMatrix)
{
	GLenum drawType;

	switch (p->drawType)
	{
	case PrimitiveType::P_POINT:
		drawType = GL_POINT;
		break;
	case PrimitiveType::P_LINE:
		drawType = GL_LINE;
		break;
	case PrimitiveType::P_FILL:
		drawType = GL_FILL;
		break;
	}
	
	SetColorGL(p->color);
	SetPointSizeGL(p->pointSize);
	SetLineWidthGL(p->lineWidth);

	GLuint modelLocation = (GLuint)(shader->uniformMap[std::string("modelMatrix")]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, modelMatrix.getAsArray());

	GLuint viewLocaton = (GLuint)(shader->uniformMap[std::string("viewMatrix")]);
	glUniformMatrix4fv(viewLocaton, 1, GL_FALSE, viewMatrix.getAsArray());

	GLuint projectionLocation = (GLuint)(shader->uniformMap[std::string("projectionMatrix")]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix.getAsArray());

	GLuint position = glGetAttribLocation(shader->shaderProgram, "position");
	glEnableVertexAttribArray(position);

	glBindBuffer(GL_ARRAY_BUFFER, p->vbo);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->ibo);

	glDrawElements(GL_POINTS, p->pointCount, GL_UNSIGNED_SHORT, (void*)0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void PrimitiveManager::SetColorGL(Vector4 color)
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