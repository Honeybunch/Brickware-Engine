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

	//Load basic models
	Shape castleShape("Models/castle.obj");

	Shape sphereShape("Models/sphere.obj");
	Shape cubeShape("Models/cube.obj");

	Shape squareShape("Models/square.obj");
	Shape circleShape("Models/circle.obj");
	Shape triangleShape("Models/triangle.obj");
	
	//Create basic meshes
	Mesh* castleMesh = new Mesh(castleShape, "Textures/castleAOTexture.png");

	Mesh* sphereMesh = new Mesh(sphereShape, "Textures/brickTexture.png");
	Mesh* cubeMesh = new Mesh(cubeShape, "Textures/boxTexture.png");

	Mesh* squareMesh = new Mesh(squareShape, "Textures/grassTexture.png");
	Mesh* circleMesh = new Mesh(circleShape, "Textures/stoneTexture.png");
	Mesh* triangleMesh = new Mesh(triangleShape, "Texture/brickTexture.png");

	//Create Camera
	GameObject* camera = new GameObject();
	camera->addComponent(new Material(shader));
	camera->addComponent(new Camera(50, 0.1f, 0.1f, 0.1f, 100.0f));
	camera->addComponent(new Spawner(sphereMesh, shader));

	//Create Light
	GameObject* light = new GameObject();
	light->getTransform()->setPosition(new Vector3(0.0f, 3.0f, 2.0f));
	light->addComponent(new Material(shader));
	light->addComponent(new Light());

	//Create Objects
	GameObject* castle = new GameObject();
	castle->getTransform()->setPosition(new Vector3(0.0f, -0.5f, 0.0f));
	castle->addComponent(new Material(shader));
	castle->addComponent(new MeshRenderer(castleMesh));

	GameObject* sphere = new GameObject();
	sphere->getTransform()->setPosition(new Vector3(-6.0f, 1.0f, -5.0f));
	sphere->addComponent(new Material(shader));
	sphere->addComponent(new MeshRenderer(sphereMesh));
	GameObject* cube = new GameObject();
	cube->getTransform()->setPosition(new Vector3(-3.0f, 1.0f, -5.0f));
	cube->addComponent(new Material(shader));
	cube->addComponent(new MeshRenderer(cubeMesh));

	GameObject* square = new GameObject();
	square->getTransform()->setPosition(new Vector3(0.0f, 1.0f, -5.0f));
	square->addComponent(new Material(shader));
	square->addComponent(new MeshRenderer(squareMesh));
	GameObject* circle = new GameObject();
	circle->getTransform()->setPosition(new Vector3(3.0f, 1.0f, -5.0f));
	circle->addComponent(new Material(shader));
	circle->addComponent(new MeshRenderer(circleMesh));
	GameObject* triangle = new GameObject();
	triangle->getTransform()->setPosition(new Vector3(6.0f, 1.0f, -5.0f));
	triangle->addComponent(new Material(shader));
	triangle->addComponent(new MeshRenderer(triangleMesh));

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
