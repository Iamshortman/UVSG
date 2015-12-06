#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "Components.hpp"
#include "Renderable.hpp"

#include "Physics/RigidBody.hpp"

#include "Rendering/ObjLoader.hpp"

UVSG* UVSG::instance;

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();
	this->renderingManager->window->setVsync(0);
	this->physicsWorld = new PhysicsWorld();

	Entity star = entitySystem.entities.create();
	star.assign<NearZoneRenderable>();
	star.assign<Transform>();
	star.component<Transform>()->setPos(vector3D(0.0, 0.0, 100.0));
	star.component<Transform>()->setScale(vector3D(10.0));
	
	Model* model = new Model();
	vector<AttributeLocation> attributes1 = { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } };
	model->shader = new ShaderProgram("res/TexturedVertex.vs", "res/TexturedFragment.fs", attributes1);
	model->texture = "res/stone.png";
	model->mesh = loadMeshFromFile("res/Sphere.obj");

	star.component<NearZoneRenderable>()->models.push_back(model);
}

void UVSG::update(double timeStep)
{
	editor.Update();

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			exitGame();
		}

		if (event.type == SDL_CONTROLLERDEVICEADDED)
		{
			continue;
		}

		if (event.type == SDL_CONTROLLERDEVICEREMOVED)
		{
			continue;
		}

		renderingManager->window->HandleEvent(event);

	} 

	//#1 step Physics
	//physicsWorld->update(timeStep);

	//#2 update Input
	//Update Key Bindings

	//#3 game logic updates
	

	//#4 audio update

	//#5 rendering
	renderingManager->window->set3dRendering();
	renderingManager->update(entitySystem, timeStep);

	//#6 UI Rendering
	renderingManager->window->set2dRendering();

}

void UVSG::exitGame()
{
	shouldClose = true;
}

const bool UVSG::getShouldClose()
{
	return shouldClose;
}

UVSG::~UVSG()
{
	delete renderingManager;
	delete physicsWorld;
}

UVSG* UVSG::getInstance()
{
	return instance;
}