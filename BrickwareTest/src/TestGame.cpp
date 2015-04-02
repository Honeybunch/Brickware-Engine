#include "TestGame.h"

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;
using namespace Utility;

TestGame::TestGame() : Game(800, 600)
{
}

bool TestGame::init()
{
	bool good = Game::init();

	if (!good)
		return false;

	//Load shader
	Shader* shader = new Shader("Shaders/VertexShader", "Shaders/PixelShader");

	//Load textures
	Texture* boxTexture = new Texture("Textures/boxTexture.bmp");
	Texture* brickTexture = new Texture("Textures/brickTexture.bmp");
	Texture* castleTexture = new Texture("Textures/castleAOTexture.bmp");
	Texture* grassTexture = new Texture("Textures/grassTexture.bmp");
	Texture* stoneTexture = new Texture("Textures/stoneTexture.bmp");
	
	//Setup materials
	Material* castleMaterial = new Material(shader);
	castleMaterial->setTexture("diffuseTexture", castleTexture);

	Material* sphereMaterial = new Material(shader);
	sphereMaterial->setTexture("diffuseTexture", brickTexture);
	Material* cubeMaterial = new Material(shader);
	cubeMaterial->setTexture("diffuseTexture", boxTexture);

	Material* squareMaterial = new Material(shader);
	squareMaterial->setTexture("diffuseTexture", grassTexture);
	Material* circleMaterial = new Material(shader);
	circleMaterial->setTexture("diffuseTexture", stoneTexture);
	Material* triangleMaterial = new Material(shader);
	triangleMaterial->setTexture("diffuseTexture", stoneTexture);

	//Load basic meshes
	Mesh* castleMesh = new Mesh("Models/castle.obj");

	Mesh* sphereMesh = new Mesh("Models/sphere.obj");
	Mesh* cubeMesh = new Mesh("Models/cube.obj");

	Mesh* squareMesh = new Mesh("Models/square.obj");
	Mesh* circleMesh = new Mesh("Models/circle.obj");
	Mesh* triangleMesh = new Mesh("Models/triangle.obj");

	//Create Lights
	GameObject* light1 = new GameObject();
	PointLight* light1Comp = new PointLight();
	light1Comp->setDiffuseColor(Vector3(0, 1, 0));
	light1Comp->setSpecularColor(Vector3(0, 1, 0));

	light1->getTransform()->setPosition(Vector3(1.0f, 0.1f, 0.0f));
	light1->addComponent(light1Comp);

	GameObject* light2 = new GameObject();
	PointLight* light2Comp = new PointLight();
	light2Comp->setDiffuseColor(Vector3(1, 0, 0));
	light2Comp->setSpecularColor(Vector3(1, 0, 0));

	light2->getTransform()->setPosition(Vector3(-1.0f, 0.1f, 0.0f));
	light2->addComponent(light2Comp);

	GameObject* light3 = new GameObject();
	PointLight* light3Comp = new PointLight();
	light3Comp->setDiffuseColor(Vector3(0, 0, .5f));
	light3Comp->setSpecularColor(Vector3(0, 0, .5f));

	light3->getTransform()->setPosition(Vector3(0.0f, 1.0f, 0.0f));
	light3->addComponent(light3Comp);

	//Create Objects
	GameObject* castle = new GameObject();
	castle->getTransform()->setPosition(Vector3(0.0f, -1.0f, 0.0f));
	castle->addComponent(new MeshRenderer(castleMesh, castleMaterial));

	GameObject* sphere = new GameObject();
	sphere->getTransform()->setPosition(Vector3(-6.0f, 1.0f, -5.0f));
	sphere->addComponent(new MeshRenderer(sphereMesh, sphereMaterial));
	Spin* sphereSpin = new Spin();
	sphereSpin->rotationDelta = Vector3(0.05f, 0.05f, 0.0f);
	sphere->addComponent(sphereSpin);
	GameObject* cube = new GameObject();
	cube->getTransform()->setPosition(Vector3(-3.0f, 1.0f, -5.0f));
	//cube->getTransform()->setScale(Vector3(0.1f, 0.1f, 0.1f));
	cube->addComponent(new MeshRenderer(cubeMesh, cubeMaterial));
	Rigidbody* cubeRigidbody = new Rigidbody();
	cubeRigidbody->setUseGravity(false);
	cubeRigidbody->setIsKinematic(false);
	cube->addComponent(cubeRigidbody);
	Spin* cubeSpin = new Spin();
	cubeSpin->rotationDelta = Vector3(0.05f, 0.0f, 0.0f);
	cube->addComponent(cubeSpin);
	cube->addComponent(new BoxCollider());

	GameObject* square = new GameObject();
	square->getTransform()->setPosition(Vector3(0.0f, 1.0f, -5.0f));
	square->addComponent(new MeshRenderer(squareMesh, squareMaterial));
	Spin* squareSpin = new Spin();
	squareSpin->rotationDelta = Vector3(0.0f, 0.05f, 0.0f);
	square->addComponent(squareSpin);
	GameObject* circle = new GameObject();
	circle->getTransform()->setPosition(Vector3(3.0f, 1.0f, -5.0f));
	circle->addComponent(new MeshRenderer(circleMesh, circleMaterial));
	Spin* circleSpin = new Spin();
	circleSpin->rotationDelta = Vector3(0.01f, 0.0f, 0.05f);
	circle->addComponent(circleSpin);
	GameObject* triangle = new GameObject();
	triangle->getTransform()->setPosition(Vector3(6.0f, 1.0f, -5.0f));
	triangle->addComponent(new MeshRenderer(triangleMesh, triangleMaterial));
	Spin* triangleSpin = new Spin();
	triangleSpin->rotationDelta = Vector3(0.05f, 0.0f, 0.05f);
	triangle->addComponent(triangleSpin);

	GameObject* nonSpinBox = new GameObject();
	Rigidbody* rigidbody = new Rigidbody();
	rigidbody->setUseGravity(false);
	nonSpinBox->addComponent(rigidbody);
	nonSpinBox->addComponent(new MeshRenderer(cubeMesh, cubeMaterial));	
	nonSpinBox->addComponent(new BoxCollider());
	nonSpinBox->getTransform()->setPosition(Vector3(0, .5f, .5f));
	nonSpinBox->getTransform()->setScale(Vector3(0.1f, 0.1f, 0.1f));

	//Create Camera
	Camera* cameraComp = new Camera(50, 0.1f, 0.1f, 0.1f, 100.0f);
	cameraComp->setActive();

	GameObject* camera = new GameObject();
	camera->addComponent(cameraComp);
	camera->addComponent(new Spawner(cube));

	for (unsigned int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->Start();

	return true;
}

void TestGame::updateScene()
{
	if (Input::getKeyDown(KeyCode::escape))
		running = false;

#ifdef _DEBUG
	if (Input::getKeyDown(KeyCode::F1) && dKeyDown == false)
	{
		Debug::Debugging = !Debug::Debugging;
		dKeyDown = true;
	}
	else if (Input::getKeyUp(KeyCode::F1))
	{
		dKeyDown = false;
	}
#endif

	for (unsigned int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->Update();
}

void TestGame::renderScene()
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->OnRender();
}

TestGame::~TestGame()
{
}
