#include "Camera.h"
#include "FrustrumCollider.h"

Camera::Camera(float FoV = 50, float width = 0.1f, float height = 0.1f, float zNear = 0.1f, float zFar = 100.0f)
{
	renderingOctree = new Octree(3, 8, Vector3(), zFar);

	this->FoV = FoV;
	this->width = width;
	this->height = height;
	this->zNear = zNear;
	this->zFar = zFar;

	//Setup speed and velocity
	speed = 0.1f;

	lookAt = new Vector3();
}

void Camera::Start()
{
	getGameObject()->addComponent(new FrustrumCollider(zNear, zFar, FoV, width / height));

	Material* material = getGameObject()->getComponent<Material>();
	
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		startD3D(material);
	else
		startGL(material);
#elif defined(USE_D3D_ONLY)
	startD3D(material);
#else
	startGL(material);
#endif
}

Vector3* Camera::getLookAt(){ return lookAt; }
void Camera::setLookAt(Vector3* lookAt){ this->lookAt = lookAt; }

void Camera::moveForward()
{
	Vector3* pos = getGameObject()->getTransform()->getPosition();
	Vector3* rot = getGameObject()->getTransform()->getRotation();

	pos->setX(pos->getX() - speed * sin(rot->getY()));
	pos->setY(pos->getY() + speed * sin(rot->getX()));
	pos->setZ(pos->getZ() -  speed * cos(rot->getY()));
}

void Camera::moveBackward()
{
	Vector3* pos = getGameObject()->getTransform()->getPosition();
	Vector3* rot = getGameObject()->getTransform()->getRotation();

	pos->setX(pos->getX() + speed * sin(rot->getY()));
	pos->setZ(pos->getZ() + speed * cos(rot->getY()));
}

void Camera::moveLeft()
{
	Vector3* pos = getGameObject()->getTransform()->getPosition();
	Vector3* rot = getGameObject()->getTransform()->getRotation();

	pos->setX(pos->getX() - speed * cos(rot->getY()));
	pos->setZ(pos->getZ() + speed * sin(rot->getY()));
}

void Camera::moveRight()
{
	Vector3* pos = getGameObject()->getTransform()->getPosition();
	Vector3* rot = getGameObject()->getTransform()->getRotation();

	pos->setX(pos->getX() + speed * cos(rot->getY()));
	pos->setZ(pos->getZ() - speed * sin(rot->getY()));
}

void Camera::Update()
{
	//Recalculate lookAt
	Vector3 position = *getGameObject()->getTransform()->getPosition();
	Vector3 forward = getGameObject()->getTransform()->getForward();

	Vector3 newLookAt = position + forward;
	
	lookAt->setX(newLookAt.getX());
	lookAt->setY(newLookAt.getY());
	lookAt->setZ(newLookAt.getZ());

	viewMatrix = calcViewMatrix();
	projectionMatrix = calcProjectionMatrix();

	//Handle Input
	if (Input::getKeyDown(KeyCode::w))
		moveForward();

	if (Input::getKeyDown(KeyCode::a))
		moveLeft();

	if (Input::getKeyDown(KeyCode::s))
		moveBackward();

	if (Input::getKeyDown(KeyCode::d))
		moveRight();

	float screenCenterX = Screen::getWidth() / 2.0f;
	float screenCenterY = Screen::getHeight() / 2.0f;

	//Adjust yaw and pitch based on how much we've deviated from the center
	float yawDiff = (screenCenterX - Input::getMousePosition().getX()) / Screen::getWidth();
	float pitchDiff = (screenCenterY - Input::getMousePosition().getY()) / Screen::getHeight();

	Transform* cameraTransform = getGameObject()->getTransform();
	Vector3* cameraRot = cameraTransform->getRotation();

	cameraTransform->getRotation()->setX(cameraRot->getX() + pitchDiff);
	cameraTransform->getRotation()->setY(cameraRot->getY() + yawDiff);

	Input::setMousePosition(Vector2(screenCenterX, screenCenterY));
}

void Camera::Render()
{	
	Material* material = getGameObject()->getComponent<Material>();
	FrustrumCollider* collider = getGameObject()->getComponent<FrustrumCollider>();

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		renderD3D(material);
	else
		renderGL(material);
#elif defined(USE_D3D_ONLY)
	renderD3D(material);
#else
	renderGL(material);
#endif

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

			//If there is no mesh renderer just render
			/*
			if (objectMesh == NULL)
			{
				object->OnRender();
				drawCalls++;
				alreadyRendered.push_back(object);

				return;
			}
			*/

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

	//std::cout << collidingNodes.size() << " , " << drawCalls << " , " << renderingOctree->nodeCount << std::endl;
}

Matrix4 Camera::calcViewMatrix()
{
	Vector3 position = *getGameObject()->getTransform()->getPosition();

	//Calculate axes 
	Vector3 zAxis = Vector3::Normalize((position - *lookAt));
	Vector3 xAxis = Vector3::Normalize(Vector3::Cross(getGameObject()->getTransform()->getUp(), zAxis));
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

	//Create view matrix;
	Matrix4 view(xAxis.getX(), yAxis.getX(), zAxis.getX(), 0, 
				 xAxis.getY(), yAxis.getY(), zAxis.getY(), 0,
				 xAxis.getZ(), yAxis.getZ(), zAxis.getZ(), 0,
				 Vector3::Dot(xAxis * -1, position),
				 Vector3::Dot(yAxis * -1, position),
				 Vector3::Dot(zAxis * -1, position), 
				 1);

	return view;
}

Matrix4 Camera::calcProjectionMatrix()
{
	float aspectRatio = width / height;

	float top = zNear * tanf(((float)M_PI / 180.0f) * (FoV * 2.0f));
	float bottom = -top;
	float right = top * aspectRatio;
	float left = -right;

	float depth = zFar - zNear;
	float q = -(zFar + zNear) / depth;
	float qn = -2 * (zFar * zNear) / depth;

	float w = 2 * zNear / width;
	w /= aspectRatio;
	float h = 2 * zNear / height;

	Matrix4 projection(w, 0, 0, 0,
					   0, h, 0, 0,
					   0, 0, q, -1,
					   0, 0, qn, 0);

	return projection;
}

void Camera::startGL(Material* material)
{
	GLint shaderProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	//Get the locations of the shader variables on the GPU
	viewMatrixPos = glGetUniformLocation(shaderProgram, "viewMatrix");
	projectionMatrixPos = glGetUniformLocation(shaderProgram, "projectionMatrix");

	lookAtPos = glGetUniformLocation(shaderProgram, "lookAt");
	eyePointPos = glGetUniformLocation(shaderProgram, "eyePoint");
	upPos = glGetUniformLocation(shaderProgram, "up");
}
void Camera::startD3D(Material* material)
{
	//TODO
}

void Camera::renderGL(Material* material)
{
	glUniformMatrix4fv(viewMatrixPos, 1, false, viewMatrix.getAsArray());
	glUniformMatrix4fv(projectionMatrixPos, 1, false, projectionMatrix.getAsArray());

	glUniform3fv(lookAtPos, 1, lookAt->getAsArray());
	glUniform3fv(eyePointPos, 1, getGameObject()->getTransform()->getPosition()->getAsArray());
	glUniform3fv(upPos, 1, getGameObject()->getTransform()->getUp().getAsArray());
}
void Camera::renderD3D(Material* material)
{
	ID3D11Buffer* constantBuffer = material->getConstantBuffer();

	
}

Camera::~Camera(void)
{
}
