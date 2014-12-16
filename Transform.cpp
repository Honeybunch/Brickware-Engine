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

Vector3 Transform::getForward(){ return forward; }
Vector3 Transform::getRight(){ return right; }
Vector3 Transform::getUp(){ return up; }

Matrix4 Transform::getModelMatrix(){ return modelMatrix; }

//Mutators
void Transform::setPosition(Vector3* newPosition){ position = newPosition; }
void Transform::setRotation(Vector3* newRotation){ rotation = newRotation; }
void Transform::setScale(Vector3* newScale){ scale = newScale; }

//Public Functions

void Transform::Update()
{
	//Calculate Model Matrix
	Matrix4 rotationX(1.0f, 0.0f, 0.0f, 0.0f,
					  0.0f, cosf(rotation->getX()), sinf(rotation->getX()), 0.0f,
					  0.0f, -sinf(rotation->getX()), cosf(rotation->getX()), 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 rotationY(cosf(rotation->getY()), 0.0f, -sinf(rotation->getY()), 0.0f,
					  0.0f, 1.0f, 0.0f, 0.0f, 
					  sinf(rotation->getY()), 0.0f, cosf(rotation->getY()), 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 rotationZ(cosf(rotation->getZ()), -sinf(rotation->getZ()), 0.0f, 0.0f,
					  sinf(rotation->getZ()), cosf(rotation->getZ()), 0.0f, 0.0f,
				      0.0f, 0.0f, 1.0f, 0.0f, 
					  0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4 rotationMat = rotationX * rotationY * rotationZ;

	Matrix4 translationMat(1.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f, 0.0f,
						   0.0f, 0.0f, 1.0f, 0.0f,
						   position->getX(), position->getY(), position->getZ(), 1.0f);

	Matrix4 scaleMat(scale->getX(), 0.0f, 0.0f, 0.0f,
					 0.0f, scale->getY(), 0.0f, 0.0f,
					 0.0f, 0.0f, scale->getZ(), 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);

	modelMatrix = scaleMat * rotationMat * translationMat;

	//Recalculate forward, right and up
	//Vector3 f, u, r;

	forward.setX(position->getX() - 1 * cosf(rotation->getX())*sinf(rotation->getY()));
	forward.setY(position->getY() + 1 * sinf(rotation->getX()));
	forward.setZ(position->getZ() + -1 * cosf(rotation->getX()) * cosf(rotation->getY()));

	up.setX( -1 * cosf((float)(rotation->getX() + M_PI/2.0f)) * sinf(rotation->getY()));
	up.setY(1 * sinf((float)(rotation->getX() + M_PI / 2.0f)));
	up.setZ(-1 * cosf((float)(rotation->getX() + M_PI / 2.0f)) * cosf(rotation->getY()));

	right = Vector3::Cross(Vector3::Normalize(forward), Vector3::Normalize(up));

	//forward = modelMatrix * Vector3(0, 0, -1);
	//right = modelMatrix * Vector3(1, 0, 0);
	//up = modelMatrix * Vector3(0, -1, 0);
}

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

}
