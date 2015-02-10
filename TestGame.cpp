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
	Texture* testTexture = new Texture("Textures/brickTexture.bmp");
	
	//Load basic meshes
	Mesh* castleMesh = new Mesh("Models/castle.obj", "Textures/castleAOTexture.png");

	Mesh* sphereMesh = new Mesh("Models/sphere.obj", "Textures/brickTexture.png");
	Mesh* cubeMesh = new Mesh("Models/cube.obj", "Textures/boxTexture.png");

	Mesh* squareMesh = new Mesh("Models/square.obj", "Textures/grassTexture.png");
	Mesh* circleMesh = new Mesh("Models/circle.obj", "Textures/stoneTexture.png");
	Mesh* triangleMesh = new Mesh("Models/triangle.obj", "Texture/brickTexture.png");

	//Create Camera
	GameObject* camera = new GameObject();
	camera->addComponent(new Material(shader));
	camera->addComponent(new Camera(50, 0.1f, 0.1f, 0.1f, 100.0f));
	camera->addComponent(new Spawner(sphereMesh, shader));

	//Create Light
	GameObject* light = new GameObject();
	light->getTransform()->setPosition(Vector3(0.0f, 3.0f, 2.0f));
	light->addComponent(new Material(shader));
	light->addComponent(new Light());

	//Create Objects
	GameObject* castle = new GameObject();
	castle->getTransform()->setPosition(Vector3(0.0f, -1.0f, 0.0f));
	castle->addComponent(new Material(shader));
	castle->addComponent(new MeshRenderer(castleMesh));

	GameObject* sphere = new GameObject();
	sphere->getTransform()->setPosition(Vector3(-6.0f, 1.0f, -5.0f));
	sphere->addComponent(new Material(shader));
	sphere->addComponent(new MeshRenderer(sphereMesh));
	Spin* sphereSpin = new Spin();
	sphereSpin->rotationDelta = Vector3(0.05f, 0.05f, 0.0f);
	sphere->addComponent(sphereSpin);
	GameObject* cube = new GameObject();
	cube->getTransform()->setPosition(Vector3(-3.0f, 1.0f, -5.0f));
	cube->addComponent(new Material(shader));
	cube->addComponent(new MeshRenderer(cubeMesh));
	Spin* cubeSpin = new Spin();
	cubeSpin->rotationDelta = Vector3(0.05f, 0.0f, 0.0f);
	cube->addComponent(cubeSpin);

	GameObject* square = new GameObject();
	square->getTransform()->setPosition(Vector3(0.0f, 1.0f, -5.0f));
	square->addComponent(new Material(shader));
	square->addComponent(new MeshRenderer(squareMesh));
	Spin* squareSpin = new Spin();
	squareSpin->rotationDelta = Vector3(0.0f, 0.05f, 0.0f);
	square->addComponent(squareSpin);
	GameObject* circle = new GameObject();
	circle->getTransform()->setPosition(Vector3(3.0f, 1.0f, -5.0f));
	circle->addComponent(new Material(shader));
	circle->addComponent(new MeshRenderer(circleMesh));
	Spin* circleSpin = new Spin();
	circleSpin->rotationDelta = Vector3(0.01f, 0.0f, 0.05f);
	circle->addComponent(circleSpin);
	GameObject* triangle = new GameObject();
	triangle->getTransform()->setPosition(Vector3(6.0f, 1.0f, -5.0f));
	triangle->addComponent(new Material(shader));
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
