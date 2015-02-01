#include "TestGame.h"


TestGame::TestGame() : Game(800, 600)
{
}

bool TestGame::init()
{
	bool good = Game::init();

	if (!good)
		return false;

	shader = new Shader("Shaders/vshader.glsl", "Shaders/fshader.glsl");

	camera = new GameObject();

	camera->addComponent(new Material(shader));
	camera->addComponent(new Camera(50, 0.1f, 0.1f, 0.1f, 100.0f));

	Shape modelShape("Models/castle.obj");
	Shape sphere(PrimitiveType::SPHERE, 10, 10);
	Shape cube(PrimitiveType::CUBE, 3, 3);

	Mesh* model = new Mesh(shader->getShaderProgram(), modelShape, "Textures/castleAOTexture.png");
	sphereMesh = new Mesh(shader->getShaderProgram(), sphere, "Textures/brickTexture.png");
	cubeMesh = new Mesh(shader->getShaderProgram(), cube, "Textures/stoneTexture.png");

	GameObject* castle = new GameObject();
	castle->getTransform()->setPosition(new Vector3(0.0f, -0.5f, 0.0f));

	castle->addComponent(new Material(shader));
	castle->addComponent(new MeshRenderer(model));

	GameObject* light = new GameObject();
	light->getTransform()->setPosition(new Vector3(0.0f, 3.0f, 2.0f));

	light->addComponent(new Material(shader));
	light->addComponent(new Light());

	//Camera::renderingOctree->addObject(castle);

	gameObjects.push_back(light);
	gameObjects.push_back(castle);
	gameObjects.push_back(camera);

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
