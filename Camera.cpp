#include "Camera.h"
#include "FrustrumCollider.h"

Octree* Camera::renderingOctree;

Camera::Camera(float FoV = 50, float width = 0.1f, float height = 0.1f, float zNear = 0.1f, float zFar = 100.0f)
{
	if (!renderingOctree)
		renderingOctree = new Octree(8, 8, Vector3(), zFar);

	this->FoV = FoV;
	this->width = width;
	this->height = height;
	this->zNear = zNear;
	this->zFar = zFar;

	//Setup speed and velocity
	speed = 0.1f;

	//Setup look, eye, and up
	lookAt = new Vector3(0, 0, 0);
	up = new Vector3(0, 1, 0);

	addComponent(new FrustrumCollider(zNear, zFar, FoV, width/height));
}

void Camera::Start()
{
	Material* material = this->getComponent<Material>();
	GLuint shaderProgram = material->getShaderProgram();

	//Get the locations of the shader variables on the GPU
	FoVPos = glGetUniformLocation(shaderProgram, "FoV");
	widthPos = glGetUniformLocation(shaderProgram, "width");
	heightPos = glGetUniformLocation(shaderProgram, "height");
	zNearPos = glGetUniformLocation(shaderProgram, "zNear");
	zFarPos = glGetUniformLocation(shaderProgram, "zFar");

	lookAtPos = glGetUniformLocation(shaderProgram, "lookAt");
	eyePointPos = glGetUniformLocation(shaderProgram, "eyePoint");
	upPos = glGetUniformLocation(shaderProgram, "up");

	GameObject::Start();
}

Vector3* Camera::getLookAt(){ return lookAt; }

void Camera::moveForward()
{
	Vector3* pos = this->transform->getPosition();
	Vector3* rot = this->transform->getRotation();

	pos->setX(pos->getX() - speed * sin(rot->getY()));
	pos->setY(pos->getY() + speed * sin(rot->getX()));
	pos->setZ(pos->getZ() -  speed * cos(rot->getY()));
}

void Camera::moveBackward()
{
	Vector3* pos = this->transform->getPosition();
	Vector3* rot = this->transform->getRotation();

	pos->setX(pos->getX() + speed * sin(rot->getY()));
	pos->setZ(pos->getZ() + speed * cos(rot->getY()));
}

void Camera::moveLeft()
{
	Vector3* pos = this->transform->getPosition();
	Vector3* rot = this->transform->getRotation();

	pos->setX(pos->getX() - speed * cos(rot->getY()));
	pos->setZ(pos->getZ() + speed * sin(rot->getY()));
}

void Camera::moveRight()
{
	Vector3* pos = this->transform->getPosition();
	Vector3* rot = this->transform->getRotation();

	pos->setX(pos->getX() + speed * cos(rot->getY()));
	pos->setZ(pos->getZ() - speed * sin(rot->getY()));
}

void Camera::Update()
{
	Vector3* pos = this->transform->getPosition();
	Vector3* rot = this->transform->getRotation();

	//Update the look at to account for position, pitch and yaw
	lookAt->setX(pos->getX() - 1*cos(rot->getX())*sin(rot->getY()));
	lookAt->setY(pos->getY() + 1 * sin(rot->getX()));
	lookAt->setZ(pos->getZ() + -1 * cos(rot->getX()) * cos(rot->getY()));

	up->setX( -1 * cos((float)(rot->getX() + M_PI/2.0f)) * sin(rot->getY()));
	up->setY(1 * sin((float)(rot->getX() + M_PI / 2.0f)));
	up->setZ(-1 * cos((float)(rot->getX() + M_PI / 2.0f)) * cos(rot->getY()));
}

void Camera::OnRender()
{	
	Material* material = this->getComponent<Material>();
	FrustrumCollider* collider = this->getComponent<FrustrumCollider>();

	if (material == NULL)
	{
		cerr << "Error in Transform; no attached shader component" << endl;
		return;
	}

	GLuint shaderProgram = material->getShaderProgram();

	glUseProgram(shaderProgram);

	glUniform1f(FoVPos, FoV);
	glUniform1f(zNearPos, zNear);
	glUniform1f(zFarPos, zFar);
	glUniform1f(widthPos, width);
	glUniform1f(heightPos, height);

	glUniform3fv(lookAtPos, 1, lookAt->getAsArray());
	glUniform3fv(eyePointPos, 1, transform->getPosition()->getAsArray());
	glUniform3fv(upPos, 1, up->getAsArray());

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Render();

	//Look through the rendering octree, see which octents collide with the camera's frustrum and then render objects in those nodes
	vector<OctreeNode*> collidingNodes = renderingOctree->getCollidingChildren(collider);

	for (unsigned int i = 0; i < collidingNodes.size(); i++)
	{
		OctreeNode* node = collidingNodes[i];
		vector<GameObject*> nodeObjects = node->getObjects();
		for (unsigned int j = 0; j < nodeObjects.size(); j++)
		{
			nodeObjects[j]->OnRender();
		}
	}

	glUseProgram(0);
}

Camera::~Camera(void)
{
}
