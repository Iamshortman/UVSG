#include "RenderingManager.hpp"

#include <iostream>
#include "Renderable.hpp"
#include "UVSG.hpp"
#include "ObjLoader.hpp"

RenderingManager::RenderingManager()
{
	string Title = "UVSG";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
	window->setBufferClearColor(0.0F, 0.0F, 0.0F, 1.0F);

	camera = Camera();
	camera.moveCameraPos(vector3D(0, 1, -10));

	//Load Skybox
	skybox = new Model();
	skybox->mesh = loadMeshFromFile("res/Skybox.obj");
	skybox->texture = "res/Skybox.png";
	skybox->shader = new ShaderProgram("res/Textured.vs", "res/Textured.fs",  { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } });

	texturePool.loadTexture(skybox->texture);
}

void RenderingManager::update(double timeStep, World* world)
{
	int width, height;
	window->getWindowSize(width, height);
	camera.setProjection(45.0f, 0.01f, 1000.0f, width, height);

	window->clearBuffer();

	//Skybox Render
	matrix4 projectionMatrix = camera.getProjectionMatrix();
	matrix4 viewMatrix = camera.getOriginViewMatrix();

	skybox->shader->setActiveProgram();
	if (skybox->texture != "")
	{
		texturePool.bindTexture(skybox->texture);
	}

	skybox->shader->setUniform("MVP", projectionMatrix * viewMatrix);
	skybox->mesh->draw(skybox->shader);
	skybox->shader->deactivateProgram();

	//Clear depth buffer so any other object in front of far objects.
	glClear(GL_DEPTH_BUFFER_BIT);

	world->render(&camera);

	//window->updateBuffer();
}

RenderingManager::~RenderingManager()
{
	window->closeWindow();
	delete window;
}
