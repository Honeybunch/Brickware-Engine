#include "Camera.h"
#include "FrustrumCollider.h"

Octree* Camera::renderingOctree;

Camera::Camera(float FoV = 50, float width = 0.1f, float height = 0.1f, float zNear = 0.1f, float zFar = 100.0f)
{
	if (!renderingOctree)
		renderingOctree = new Octree(3, 8, Vector3(), zFar);

	this->FoV = FoV;
	this->width = width;
	this->height = height;
	this->zNear = zNear;
	this->zFar = zFar;

	//Setup speed and velocity
	speed = 0.1f;

	lookAt = new Vector3();

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
void Camera::setLookAt(Vector3* lookAt){ this->lookAt = lookAt; }

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
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Update();

	//Recalculate lookAt
	Vector3 position = *transform->getPosition();
	Vector3 forward = transform->getForward();

	Vector3 newLookAt = position + forward;
	
	lookAt->setX(newLookAt.getX());
	lookAt->setY(newLookAt.getY());
	lookAt->setZ(newLookAt.getZ());
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
	glUniform3fv(upPos, 1, transform->getUp().getAsArray());

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->Render();

	//Look through the rendering octree, see which octents collide with the camera's frustrum and then render objects in those nodes
	vector<OctreeNode*> collidingNodes = renderingOctree->getCollidingChildren(collider);
	vector<GameObject*> alreadyRendered;

	int drawCalls = 0;

	for (unsigned int i = 0; i < collidingNodes.size(); i++)
	{
		OctreeNode* node = collidingNodes[i];
		vector<GameObject*> nodeObjects = node->getObjects();
		for (unsigned int j = 0; j < nodeObjects.size(); j++)
		{
			GameObject* object = nodeObjects[j];
			MeshRenderer* objectMesh = object->getComponent<MeshRenderer>();
			Bounds* meshBounds = objectMesh->getBounds();

			bool isColliding = collider->isColliding(meshBounds);

			if (isColliding)
			{
				bool rendered = false;

				for (unsigned int j = 0; j < alreadyRendered.size(); j++)
				{
					if (object == alreadyRendered[j])
						rendered = true;
				}

				if (rendered == false)
				{
					object->OnRender();
					drawCalls++;
					alreadyRendered.push_back(object);
				}
			}
		}
	}

	std::cout << collidingNodes.size() << " , " << drawCalls << " , " << renderingOctree->nodeCount << std::endl;

	glUseProgram(0);
}

Camera::~Camera(void)
{
}
