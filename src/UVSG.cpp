#include <stddef.h>  // defines NULL
#include "UVSG.hpp"
#include "World/EntityManager.hpp"
#include "Rendering/EntityRenderer.hpp"
#include "Rendering/ObjLoader.hpp"

#include "Components/TimeToLive.hpp"
#include "Components/ShipFlightControl.hpp"

UVSG* UVSG::instance;

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();
	this->renderingManager->window->setVsync(1);
	this->baseWorld = new World();

	if (true)
	{
		Entity* ship = EntityManager::instance()->createNewEntity();
		ship->addComponent("FlightControl", new ShipFlightControl(SDL_JoystickOpen(0)));
		ship->addComponent("CameraMount", new Component());

		ship->addToWorld(baseWorld);
		ship->m_transform = Transform(vector3D(0.0, 5.0, 0.0));

		Model* model1 = new Model();
		model1->localOffset = Transform();
		model1->shader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
		model1->mesh = loadMaterialMeshFromFile("res/ShipParts/", "Ship_Cobra.obj");
		ship->tempModels.push_back(model1);

		btCompoundShape* shape = new btCompoundShape();
		shape->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 2.5)), new btBoxShape(btVector3(0.5, 0.5, 1.0)));
		shape->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0.1875, 0.5)), new btBoxShape(btVector3(0.5, 0.6875, 1.0)));
		shape->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -1.75)), new btBoxShape(btVector3(0.5, 0.5, 2.5 / 2)));

		ship->addRigidBody(new RigidBody(shape, 100.0));
	}

	m_renderer = new EntityRenderer();
}

void UVSG::update(double timeStep)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		if (event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			exitGame();
		}

		/*if (event.type == SDL_CONTROLLERDEVICEADDED)
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
		}*/

		renderingManager->window->HandleEvent(event);

	}

	baseWorld->updateWorld(timeStep);

	vector<Entity*> entities = EntityManager::instance()->getAllEntities();

	for (Entity* entity : entities)
	{
		if (entity != nullptr && entity->hasComponent("CameraMount"))
		{
			vector3D pos = entity->m_transform.m_position;
			printf("Pos: ");
			printVec(pos);
			printEndLine();

			pos += entity->m_transform.m_orientation * vector3D(0, 0.7, 0);

			renderingManager->camera.setCameraTransform(pos, entity->m_transform.m_orientation);
		}
	}

	//#5 rendering
	renderingManager->window->set3dRendering();
	renderingManager->update(timeStep, baseWorld);

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
	delete baseWorld;
}

UVSG* UVSG::getInstance()
{
	return instance;
}
