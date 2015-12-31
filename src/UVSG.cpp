#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "Components.hpp"
#include "Renderable.hpp"

#include "Physics/RigidBody.hpp"

#include "Rendering/ObjLoader.hpp"
#include "Components/PlayerControl.hpp"

UVSG* UVSG::instance;

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();
	this->renderingManager->window->setVsync(1);
	this->physicsWorld = new PhysicsWorld();

	entitySystem.systems.add<PlayerControlSystem>();
	entitySystem.systems.configure();

	Transform camTransform;
	camTransform.setPos(vector3D(-10.0f, 15.0f, -10.0f));
	//camTransform.m_orientation = glm::angleAxis(toRad(30.0), vector3D(1, 0, 0)) * camTransform.m_orientation;
	camTransform.m_orientation = glm::angleAxis(toRad(45.0), vector3D(0, 1, 0)) * camTransform.m_orientation;
	this->renderingManager->camera.setCameraTransform(camTransform.getPos(), camTransform.getOrientation());

	m_camera = entitySystem.entities.create();
	m_camera.assign<Transform>();
	m_camera.assign<PlayerControlComponent>(3.0, 3.0);
	m_camera.component<Transform>()->setTransform(camTransform);

	Entity star = entitySystem.entities.create();
	star.assign<FarZoneRenderable>();
	star.assign<Transform>();
	star.component<Transform>()->setPos(vector3D(200000000.0, 0.0, 200000000.0));
	star.component<Transform>()->setScale(vector3D(60000000.0));

	Model* model = new Model();
	vector<AttributeLocation> attributes1 = { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } };
	model->shader = new ShaderProgram("res/TexturedVertex.vs", "res/TexturedFragment.fs", attributes1);
	model->texture = "res/StarRed.png";
	model->mesh = loadMeshFromFile("res/Sphere.obj");

	star.component<FarZoneRenderable>()->models.push_back(model);

	Model* model1 = new Model();
	model1->shader = model->shader;
	model1->texture = model->texture;
	model1->mesh = loadMeshFromFile("res/Cube.obj");

	editor.tempModel = model1;
	editor.shader = new ShaderProgram("res/ColoredVertex.vs", "res/ColoredFragment.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Color" } });
}

void UVSG::update(double timeStep)
{
	entitySystem.systems.update_all(timeStep);
	editor.Update();

	this->renderingManager->camera.setCameraTransform(m_camera.component<Transform>()->getPos(), m_camera.component<Transform>()->getOrientation());

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			exitGame();
		}

		if (event.type == SDL_CONTROLLERDEVICEADDED)
		{
			int i = event.cdevice.which;

			joystick = SDL_JoystickOpen(i);
			if (SDL_IsGameController(i))
			{
				controller = SDL_GameControllerOpen(i);
				printf("%s \n", SDL_JoystickName(joystick));
			}

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
