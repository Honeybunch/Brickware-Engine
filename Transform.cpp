#include "Transform.h"
#include "GameObject.h"

Transform::Transform()
{
	position = new Vector3(0.0f, 0.0f, 0.0f);
	rotation = new Vector3(0.0f, 0.0f, 0.0f);
	scale = new Vector3(1.0f, 1.0f, 1.0f);
}

//Accessors
Vector3* Transform::getPosition(){ return position; }
Vector3* Transform::getRotation(){ return rotation; }
Vector3* Transform::getScale(){ return scale; }

//Mutators
void Transform::setPosition(Vector3* newPosition){ position = newPosition; }
void Transform::setRotation(Vector3* newRotation){ rotation = newRotation; }
void Transform::setScale(Vector3* newScale){ scale = newScale; }

//Public Functions

void Transform::Render()
{
	GameObject* go = this->getGameObject();
	Material* material = go->getComponent<Material>();

	GLuint shaderProgram = material->getShaderProgram();

	glUniform3fv(material->getPositionLocation(), 1, position->getAsArray());
	glUniform3fv(material->getRotationLocation(), 1, rotation->getAsArray());
	glUniform3fv(material->getScaleLocation(), 1, scale->getAsArray());
}

Transform::~Transform()
{
	delete position;
	delete rotation;
	delete scale;
}
