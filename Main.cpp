/*
Brickware Engine, a simple 3D game engine 

Copyright(C) 2014 Arsen Tufankjian

This program is free software; you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.
*/


#include "TestGame.h"

bool running;
float interpolation;
int ticks;

Shader* shader;

Mesh* sphereMesh;
Mesh* cubeMesh;

GameObject* camera;

vector<GameObject*> gameObjects;

GLFWwindow* window;

void createShapes()
{
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
}

//Take the sphere shape, make a model out of it at a given position and push it to the mesh vector
void spawnSphere()
{
	GameObject* newSphere = new GameObject();

	newSphere->addComponent(new Material(shader));
	newSphere->addComponent(new MeshRenderer(sphereMesh));
	newSphere->addComponent(new SphereCollider(new Vector3(), 0.1f));

	Vector3* spherePos = new Vector3(*(camera->getComponent<Camera>()->getLookAt()));

	newSphere->getTransform()->setPosition(spherePos);
	newSphere->getTransform()->setScale(new Vector3(0.1f, 0.1f, 0.1f));
	
	newSphere->Start();

	//Check if the spheres are colliding
	Collider* collider = newSphere->getComponent<Collider>();

	bool collided = false;

	if (collider)
	{
		for (unsigned int i = 0; i < gameObjects.size(); i++)
		{
			Collider* otherCollider = gameObjects[i]->getComponent<Collider>();

			//If the game object we're looking at has a collider
			if (otherCollider)
			{
				if (otherCollider->isColliding(collider))
				{
					collided = true;
					break;
				}
			}
		}
	}

	if (collided)
	{
		delete newSphere;
	}
	else
	{
		gameObjects.push_back(newSphere);
		camera->getComponent<Camera>()->renderingOctree->addObject(newSphere);
	}
}

void display()
{
	//Drawing settings
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Update Game Objects
	for (unsigned int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->Update();

	//Draw Game Objects
	for (unsigned int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->OnRender();

	//camera->OnRender();

	//Swap buffers
	glfwSwapBuffers(window);
}

void update()
{

}

void mouseMove(float x, float y)
{
	float screenCenterX = 512.0f / 2.0f;
	float screenCenterY = 512.0f / 2.0f;

	//Adjust yaw and pitch based on how much we've deviated from the center
	float yawDiff = (screenCenterX - x) / 512.0f;
	float pitchDiff = (screenCenterY - y) / 512.0f;

	Transform* cameraTransform = camera->getTransform();
	Vector3* cameraRot = cameraTransform->getRotation();

	cameraTransform->getRotation()->setX(cameraRot->getX() + pitchDiff);
	cameraTransform->getRotation()->setY(cameraRot->getY() + yawDiff);

	glfwSetCursorPos(window, screenCenterX, screenCenterY);
}

void handleInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		running = false;

	Camera* cameraComponent = camera->getComponent<Camera>();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_REPEAT)
		cameraComponent->moveForward();

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_REPEAT)
		cameraComponent->moveLeft();

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_REPEAT)
		cameraComponent->moveBackward();

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_REPEAT)
		cameraComponent->moveRight();

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_REPEAT)
		spawnSphere();

	double mouseX = 0;
	double mouseY = 0;

	glfwGetCursorPos(window, &mouseX, &mouseY);

	mouseMove((float)mouseX, (float)mouseY);
}

void run()
{
	float ticksPerSecond = 25.0f;
	int skipTicks = (int)(1000.0f / ticksPerSecond);
	float maxFrameskip = 5;

	int nextGameTick = 0;

	int loops = 0;

	running = true;

	while (running)
	{
		ticks++;

		//Poll for events
		glfwPollEvents();

		handleInput();

		//Update logic 25 times per second
		loops = 0;
		while (ticks > nextGameTick && loops < maxFrameskip)
		{
			update();

			nextGameTick += skipTicks;
			loops++;
		}

		//Calculate interpolation
		interpolation = (float)(((float)ticks + skipTicks - nextGameTick) / (float)skipTicks);

		display();
	}
}


// OpenGL initialization 
void init()
{    
	//OpenGL initialization
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
    glClearColor(1.0, 1.0, 1.0, 1.0);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

#ifdef _WIN32
	glewInit();
#endif
    
	shader = new Shader("Shaders/vshader.glsl", "Shaders/fshader.glsl");

    //Create scene
    createShapes();
}

int main (int argc, char **argv)
{
	/*
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Attempt initialization
	if (!glfwInit())
		return -1;

	//Create window
	window = glfwCreateWindow(512, 512, "Brickware-Test", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -2;
	}

	//Make Context
	glfwMakeContextCurrent(window);

	//Init GL context
	init();

	//Run Game loop
	run();

	glfwTerminate();
	*/

	TestGame game;

	if (!game.init())
		return -1;

	return game.run();
}