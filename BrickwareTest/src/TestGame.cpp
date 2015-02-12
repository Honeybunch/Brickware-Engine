#include "TestGame.h"

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

	//Create Camera
	GameObject* camera = new GameObject();
	camera->addComponent(new Material(shader));
	camera->addComponent(new Camera(50, 0.1f, 0.1f, 0.1f, 100.0f));
	camera->addComponent(new Spawner(sphereMesh, shader));

	//Create Light
	GameObject* light = new GameObject();
	light->getTransform()->setPosition(Vector3(0.0f, 1.0f, 0.0f));
	light->addComponent(new Material(shader));
	light->addComponent(new Light());

	//Create Objects
	GameObject* castle = new GameObject();
	castle->getTransform()->setPosition(Vector3(0.0f, -1.0f, 0.0f));
	castle->addComponent(castleMaterial);
	castle->addComponent(new MeshRenderer(castleMesh));

	GameObject* sphere = new GameObject();
	sphere->getTransform()->setPosition(Vector3(-6.0f, 1.0f, -5.0f));
	sphere->addComponent(sphereMaterial);
	sphere->addComponent(new MeshRenderer(sphereMesh));
	Spin* sphereSpin = new Spin();
	sphereSpin->rotationDelta = Vector3(0.05f, 0.05f, 0.0f);
	sphere->addComponent(sphereSpin);
	GameObject* cube = new GameObject();
	cube->getTransform()->setPosition(Vector3(-3.0f, 1.0f, -5.0f));
	cube->addComponent(cubeMaterial);
	cube->addComponent(new MeshRenderer(cubeMesh));
	Spin* cubeSpin = new Spin();
	cubeSpin->rotationDelta = Vector3(0.05f, 0.0f, 0.0f);
	cube->addComponent(cubeSpin);

	GameObject* square = new GameObject();
	square->getTransform()->setPosition(Vector3(0.0f, 1.0f, -5.0f));
	square->addComponent(squareMaterial);
	square->addComponent(new MeshRenderer(squareMesh));
	Spin* squareSpin = new Spin();
	squareSpin->rotationDelta = Vector3(0.0f, 0.05f, 0.0f);
	square->addComponent(squareSpin);
	GameObject* circle = new GameObject();
	circle->getTransform()->setPosition(Vector3(3.0f, 1.0f, -5.0f));
	circle->addComponent(circleMaterial);
	circle->addComponent(new MeshRenderer(circleMesh));
	Spin* circleSpin = new Spin();
	circleSpin->rotationDelta = Vector3(0.01f, 0.0f, 0.05f);
	circle->addComponent(circleSpin);
	GameObject* triangle = new GameObject();
	triangle->getTransform()->setPosition(Vector3(6.0f, 1.0f, -5.0f));
	triangle->addComponent(triangleMaterial);
	triangle->addComponent(new MeshRenderer(triangleMesh));
	Spin* triangleSpin = new Spin();
	triangleSpin->rotationDelta = Vector3(0.05f, 0.0f, 0.05f);
	triangle->addComponent(triangleSpin);

	for (unsigned int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->Start();

	return true;
}

void TestGame::updateScene()
{
	if (Input::getKeyDown(KeyCode::escape))
		running = false;

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
