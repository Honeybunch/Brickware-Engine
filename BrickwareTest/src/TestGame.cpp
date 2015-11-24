#include "TestGame.hpp"

using namespace Brickware;
using namespace Core;
using namespace Graphics;
using namespace Math;
using namespace Utility;

TestGame::TestGame() : Game() {}

TestGame* TestGame::currentGame = nullptr;

bool TestGame::init() 
{
	bool good = Game::init();

	if (!good)
		return false;

	Screen::SetResolution(1280, 800, false);
	Screen::SetCursorVisible(false);

	// Test Renderer Info
	std::cout << "API Version: " << RendererInfo::GetAPIVersion() << std::endl;
	std::cout << "Card Vendor: " << RendererInfo::GetVendor() << std::endl;
	std::cout << "Renderer: " << RendererInfo::GetRenderer() << std::endl;
	std::cout << "Shading Language Version: "
		<< RendererInfo::GetShadingLanguageVersion() << std::endl;
	std::vector<std::string> SupportedShadingLanguageVersions =
		RendererInfo::GetAllSupportedShadingLanguageVersions();
	std::cout << "All Supported Shading Language Versions: " << std::endl;
	for (unsigned int i = 0; i < SupportedShadingLanguageVersions.size(); i++)
		std::cout << "\t" << SupportedShadingLanguageVersions[i] << std::endl;

	// Test JSON loading
	JSONObject *object = JSONParser::DecodeJSONFromFile("Data/test.json");

	if (object)
	{
		bool boolean = object->getValue<bool>("testBool");
		char *string = object->getValue<char *>("testStr");
		int integer = object->getValue<int>("testInt");
		float floatingPoint = object->getValue<float>("testFloat");
		bool testNULL = object->getValue<int>("testNull") != 0;
		JSONObject *nestedObject = object->getValue<JSONObject *>("testObject");
		int nestedInt = nestedObject->getValue<int>("testObjectInt");
		char *nestedString = nestedObject->getValue<char *>("testObjectStr");
		std::vector<JSONValue> *array =
			object->getValue<std::vector<JSONValue> *>("testArray");
		std::vector<JSONValue> *objectArray =
			object->getValue<std::vector<JSONValue> *>("testStrArray");

		std::cout << "{" << std::endl;
		std::cout << "\t" << boolean << std::endl;
		std::cout << "\t" << string << std::endl;
		std::cout << "\t" << integer << std::endl;
		std::cout << "\t" << floatingPoint << std::endl;
		std::cout << "\t" << testNULL << std::endl;
		std::cout << "\t\t" << nestedInt << std::endl;
		std::cout << "\t\t" << nestedString << std::endl;

		std::cout << "\t[";
		for (unsigned int i = 0; i < array->size(); i++)
			std::cout << (int)array->at(i) << ',';
		std::cout << "]" << std::endl;

		for (unsigned int i = 0; i < objectArray->size(); i++) {
			JSONObject *arrayObj = objectArray->at(i);

			std::cout << "\t{" << std::endl;
			std::cout << "\t\tindex:" << arrayObj->getValue<int>("index")
				<< std::endl;
			std::cout << "\t}," << std::endl;
		}

		std::cout << "}" << std::endl;
		std::cout << std::endl;
	}

	// Load shader
	Shader *shader = new Shader("Shaders/VertexShader", "Shaders/PixelShader");

	// Load textures
	Texture *boxTexture =
		new Texture("Textures/boxTexture.bmp", TextureColorSpace::LINEAR);
	Texture *brickTexture =
		new Texture("Textures/brickTexture.bmp", TextureColorSpace::LINEAR);
	Texture *castleTexture =
		new Texture("Textures/castleAOTexture.bmp", TextureColorSpace::LINEAR);
	Texture *grassTexture =
		new Texture("Textures/grassTexture.bmp", TextureColorSpace::LINEAR);
	Texture *stoneTexture =
		new Texture("Textures/stoneTexture.bmp", TextureColorSpace::LINEAR);
	Texture *bitTestTexture =
		new Texture("Textures/32BitTest.bmp", TextureColorSpace::LINEAR);

	// Setup materials
	Material *castleMaterial = new Material(shader);
	castleMaterial->setTexture("a_diffuseTexture", castleTexture);

	Material *sphereMaterial = new Material(shader);
	sphereMaterial->setTexture("a_diffuseTexture", bitTestTexture);
	Material *cubeMaterial = new Material(shader);
	cubeMaterial->setTexture("a_diffuseTexture", boxTexture);

	Material *squareMaterial = new Material(shader);
	squareMaterial->setTexture("a_diffuseTexture", grassTexture);
	Material *circleMaterial = new Material(shader);
	circleMaterial->setTexture("a_diffuseTexture", stoneTexture);
	Material *triangleMaterial = new Material(shader);
	triangleMaterial->setTexture("a_diffuseTexture", stoneTexture);

	// Load basic meshes
	Mesh *castleMesh = new Mesh("Models/castle.obj");

	Mesh *sphereMesh = new Mesh("Models/sphere.obj");
	Mesh *cubeMesh = new Mesh("Models/cube.obj");

	Mesh *squareMesh = new Mesh("Models/cloth.obj");
	Mesh *circleMesh = new Mesh("Models/circle.obj");
	Mesh *triangleMesh = new Mesh("Models/triangle.obj");

	// Create Lights
	GameObject *light1 = new GameObject();
	Light *light1Comp = new Light(LightType::POINT);
	light1Comp->setDiffuseColor(Vector3(.3f, .3f, .3f));
	light1Comp->setSpecularColor(Vector3(.6f, .6f, .6f));

	light1Comp->setShadowMapQuality(ShadowQuality::High);
	light1Comp->setShadowBias(0.00005f);
	light1Comp->setShadowStrength(0.5f);

	light1->getTransform()->setPosition(Vector3(1.0f, 0.1f, 0.0f));
	light1->addComponent(light1Comp);

	// GameObject* light2 = new GameObject();
	// Light* light2Comp = new Light(LightType::POINT);
	// light2Comp->setDiffuseColor(Vector3(.3f, .3f, .3f));
	// light2Comp->setSpecularColor(Vector3(.6f, .6f, .6f));
	//
	// light2Comp->setShadowMapQuality(ShadowQuality::Medium);
	// light2Comp->setShadowBias(0.05f);
	// light2Comp->setShadowStrength(0.5f);
	//
	// light2->getTransform()->setPosition(Vector3(-1.0f, 0.1f, 0.0f));
	// light2->addComponent(light2Comp);

	GameObject *dirLight = new GameObject();
	Light *dirLightComp = new Light(LightType::DIRECTIONAL);
	dirLightComp->setDirection(Vector3(0, -0.9f, 1.0f));
	dirLightComp->setDiffuseColor(Vector3(0.5f, 0.5f, 0.5f));
	dirLightComp->setSpecularColor(Vector3(0.5f, 0.5f, 0.5f));

	dirLightComp->setShadowMapQuality(ShadowQuality::High);
	dirLightComp->setShadowBias(0.00005f);
	dirLightComp->setShadowStrength(0.7f);

	dirLight->getTransform()->setPosition(Vector3(0, 1, 0));
	dirLight->addComponent(dirLightComp);

	// Create Objects
	GameObject *castle = new GameObject();
	castle->getTransform()->setPosition(Vector3(0.0f, -1.0f, 0.0f));
	castle->addComponent(new MeshRenderer(castleMesh, castleMaterial));

	GameObject *sphere = new GameObject();
	sphere->getTransform()->setPosition(Vector3(-6.0f, 1.0f, -5.0f));
	sphere->addComponent(new MeshRenderer(sphereMesh, sphereMaterial));
	sphere->addComponent(new SphereCollider(Vector3(), 1.0f));
	Spin *sphereSpin = new Spin();
	sphereSpin->rotationDelta = Vector3(0.05f, 0.05f, 0.0f);
	sphere->addComponent(sphereSpin);

	GameObject *cube = new GameObject();
	cube->getTransform()->setPosition(Vector3(-3.0f, 1.0f, -5.0f));
	cube->getTransform()->setScale(Vector3(1.0f, 1.0f, 1.0f));
	cube->addComponent(new MeshRenderer(cubeMesh, cubeMaterial));
	Rigidbody *cubeRigidbody = new Rigidbody();
	cubeRigidbody->setUseGravity(false);
	cubeRigidbody->setMass(1.0f);
	cubeRigidbody->setIsKinematic(false);
	cube->addComponent(cubeRigidbody);
	Spin *cubeSpin = new Spin();
	cubeSpin->rotationDelta = Vector3(0.05f, 0.0f, 0.0f);
	cube->addComponent(cubeSpin);
	cube->addComponent(new BoxCollider());

	GameObject *square = new GameObject();
	square->getTransform()->setPosition(Vector3(0.0f, 2.5f, -5.0f));
	square->addComponent(new MeshRenderer(squareMesh, squareMaterial));
	GameObject *circle = new GameObject();
	circle->getTransform()->setPosition(Vector3(3.0f, 1.0f, -5.0f));
	circle->addComponent(new MeshRenderer(circleMesh, circleMaterial));
	Spin *circleSpin = new Spin();
	circleSpin->rotationDelta = Vector3(0.01f, 0.0f, 0.05f);
	circle->addComponent(circleSpin);
	GameObject *triangle = new GameObject();
	triangle->getTransform()->setPosition(Vector3(6.0f, 1.0f, -5.0f));
	triangle->addComponent(new MeshRenderer(triangleMesh, triangleMaterial));
	Spin *triangleSpin = new Spin();
	triangleSpin->rotationDelta = Vector3(0.05f, 0.0f, 0.05f);
	triangle->addComponent(triangleSpin);

	GameObject *nonSpinBox = new GameObject();
	Rigidbody *rigidbody = new Rigidbody();
	rigidbody->setUseGravity(false);
	rigidbody->setMass(1.0f);
	nonSpinBox->addComponent(rigidbody);
	nonSpinBox->addComponent(new MeshRenderer(cubeMesh, cubeMaterial));
	nonSpinBox->addComponent(new BoxCollider());
	nonSpinBox->getTransform()->setPosition(Vector3(0, 1.0f, -10.0f));
	nonSpinBox->getTransform()->setScale(Vector3(1.0f, 1.0f, 1.0f));

	//Create Game Manager
	GameObject* gameManager = new GameObject();
	GameManager* gameManagerComp = new GameManager();
	gameManager->addComponent(gameManagerComp);

	// Create Camera
	float fov = (60.0f * 180.0f) / (float)M_PI;

	Camera *cameraComp = new Camera(fov, (float)Screen::GetWidth(),
		(float)Screen::GetHeight(), 0.1f, 100.0f);
	cameraComp->SetActive();

	GameObject *camera = new GameObject();
	camera->addComponent(cameraComp);
	camera->addComponent(new Spawner(cube));
	camera->addComponent(new FirstPersonController());

	for (unsigned int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->Start();

	currentGame = this;

	return true;
}

void TestGame::Shutdown()
{
	if (currentGame != nullptr)
		currentGame->running = false;
}

TestGame::~TestGame() {}
