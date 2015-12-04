#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "RenderingManager.hpp"
#include "Components.hpp"
#include "Renderable.hpp"

#include "RigidBody.hpp"

UVSG* UVSG::instance;

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();
	this->renderingManager->window->setVsync(0);
	this->physicsWorld = new PhysicsWorld();

	Entity star = entitySystem.entities.create();
	star.assign<FarZoneRenderable>();
	star.assign<Transform>();
	star.component<Transform>()->setPos(f64vec3(0.0, 0.0, 10000000.0));
	star.component<Transform>()->setScale(f64vec3(200000.0));
	
	Model* model = new Model();
	vector<AttributeLocation> attributes1 = { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } };
	model->shader = new ShaderProgram("TexturedVertex.vs", "TexturedFragment.fs", attributes1);
	model->texture = "stone.png";
	star.component<FarZoneRenderable>()->models.push_back(model);
}

void UVSG::update(double timeStep)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			exitGame();
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