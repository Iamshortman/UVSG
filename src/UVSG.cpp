#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "RenderingManager.hpp"
#include "Components.hpp"
#include "TimeToLive.hpp"
#include "VoxelSystem.hpp"
#include "PlayerControl.hpp"

UVSG* UVSG::instance;

void push3(vector<unsigned int>* vector, unsigned int a, unsigned int b, unsigned int c)
{
	vector->push_back(a - 1);
	vector->push_back(b - 1);
	vector->push_back(c - 1);
}

UVSG::UVSG()
{
	instance = this;

	this->renderingManager = new RenderingManager();
	//this->renderingManager->window->setVsync(0);

	this->physicsWorld = new PhysicsWorld();

	entitySystem.systems.add<TimeToLiveSystem>();
	entitySystem.systems.add<VoxelSystem>();
	entitySystem.systems.add<PlayerControlSystem>();
	entitySystem.systems.configure();

	Entity camEntity = entitySystem.entities.create();
	camEntity.assign<Transform>();
	camEntity.assign<CameraLock>();
	camEntity.assign<PlayerControlComponent>(3.0f, 3.0f);
	camEntity.component<Transform>()->position = vector3(0.0, 10.0f, -25.0f);

	Entity entity = entitySystem.entities.create();
	entity.assign<MeshComponent>();
	entity.assign<Transform>();

	vector<vector3> vertices1 = vector<vector3>();
	vector<vector3> colors1 = vector<vector3>();
	vector<vector3> normals1 = vector<vector3>();
	vector<unsigned int> indices1 = vector<unsigned int>();

	vertices1.push_back(vector3(50.0f, 0.0f, 50.0f));
	vertices1.push_back(vector3(50.0f, 0.0f, -50.0f));
	vertices1.push_back(vector3(-50.0f, 0.0f, -50.0f));
	vertices1.push_back(vector3(-50.0f, 0.0f, 50.0f));
	colors1.push_back(vector3(1.0F, 1.0F, 0.0F));
	colors1.push_back(vector3(0.0F, 1.0F, 1.0F));
	colors1.push_back(vector3(1.0F, 0.0F, 1.0F));
	colors1.push_back(vector3(0.0F, 1.0F, 1.0F));
	push3(&indices1, 1, 2, 3);
	push3(&indices1, 3, 4, 1);
	normals1.push_back(vector3(0.0F, 1.0F, 0.0F));
	normals1.push_back(vector3(0.0F, 1.0F, 0.0F));
	normals1.push_back(vector3(0.0F, 1.0F, 0.0F));
	normals1.push_back(vector3(0.0F, 1.0F, 0.0F));

	ComponentHandle<MeshComponent> componentMesh = entity.component<MeshComponent>();
	componentMesh->mesh.addVertices(vertices1, colors1, normals1, indices1);
	componentMesh->offset = vector3(0.0f, 0.0f, 0.0f);
	entity.assign<RigidBody>(physicsWorld, entity, new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0), 0.0f);

	Entity voxelObject = entitySystem.entities.create();
	voxelObject.assign<Transform>();
	voxelObject.assign<Velocity>();
	voxelObject.component<Transform>()->position = vector3(0.0f, 10.0f, 0.0f);
	voxelObject.component<Transform>()->orientation = quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	voxelObject.assign<VoxelComponent>();
	voxelObject.assign<RigidBody>(physicsWorld, voxelObject, new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), 10000.0f);
	voxelObject.assign<MeshComponent>();

	for (unsigned int i = 0; i < voxelObject.component<VoxelComponent>()->chunkSize; i++)
	{
		for (unsigned int j = 0; j < voxelObject.component<VoxelComponent>()->chunkSize; j++)
		{
			for (unsigned int k = 0; k < voxelObject.component<VoxelComponent>()->chunkSize; k++)
			{
				voxelObject.component<VoxelComponent>()->setBlock(i, j, k, 1);
			}
		}
	}

}


void UVSG::update(float timeStep)
{


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
	physicsWorld->update(entitySystem, timeStep);

	static int lastButton = 0;
	int Button = 0;

	int trigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	//If the trigger is more than half pressed.
	if (trigger > 16390)
	{
		Button = 1;
	}
	else
	{
		Button = 0;
	}

	if (Button && !lastButton)
	{
		if (haptic != NULL)
		{
			// Initialize simple rumble
			SDL_HapticRumbleInit(haptic);

			if (SDL_HapticRumblePlay(haptic, 1.0, 125) != 0)
			{

			}
		}

		Entity entity1 = entitySystem.entities.create();
		entity1.assign<MeshComponent>();
		entity1.assign<Transform>();
		entity1.assign<Velocity>();
		entity1.assign<RigidBody>(physicsWorld, entity1, new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)), 10.0f);

		entity1.assign<TimeToLiveComponent>(15.0f);

		entity1.component<Transform>()->position = renderingManager->camera.getForward() + renderingManager->camera.getPos();
		entity1.component<Velocity>()->linearVelocity = renderingManager->camera.getForward() * 50.0f;

		vector<vector3> vertices = vector<vector3>();
		vertices.push_back(vector3(1.0f, -1.0f, -1.0f));
		vertices.push_back(vector3(1.0f, -1.0f, 1.0f));
		vertices.push_back(vector3(-1.0f, -1.0f, 1.0f));
		vertices.push_back(vector3(-1.0f, -1.0f, -1.0f));
		vertices.push_back(vector3(1.0f, 1.0f, -1.0f));
		vertices.push_back(vector3(1.0f, 1.0f, 1.0f));
		vertices.push_back(vector3(-1.0f, 1.0f, 1.0f));
		vertices.push_back(vector3(-1.0f, 1.0f, -1.0f));


		vector<vector3> colors = vector<vector3>();
		colors.push_back(vector3(1.0F, 1.0F, 0.0F));
		colors.push_back(vector3(0.0F, 1.0F, 1.0F));
		colors.push_back(vector3(1.0F, 0.0F, 1.0F));
		colors.push_back(vector3(0.0F, 1.0F, 1.0F));
		colors.push_back(vector3(1.0F, 1.0F, 0.0F));
		colors.push_back(vector3(0.0F, 1.0F, 1.0F));
		colors.push_back(vector3(1.0F, 0.0F, 1.0F));
		colors.push_back(vector3(0.0F, 1.0F, 1.0F));

		vector<vector3> normals = vector<vector3>();

		vector<unsigned int> indices = vector<unsigned int>();
		push3(&indices, 5, 8, 6);//TOP
		push3(&indices, 8, 7, 6);
		normals.push_back(vector3(0.0F, 1.0F, 0.0F));
		normals.push_back(vector3(0.0F, 1.0F, 0.0F));
		normals.push_back(vector3(0.0F, 1.0F, 0.0F));

		push3(&indices, 1, 2, 4);//Bottom
		push3(&indices, 2, 3, 4);
		normals.push_back(vector3(0.0F, -1.0F, 0.0F));
		normals.push_back(vector3(0.0F, -1.0F, 0.0F));
		normals.push_back(vector3(0.0F, -1.0F, 0.0F));

		push3(&indices, 2, 6, 3);//Back
		push3(&indices, 6, 7, 3);
		normals.push_back(vector3(0.0F, 0.0F, -1.0F));
		normals.push_back(vector3(0.0F, 0.0F, -1.0F));
		normals.push_back(vector3(0.0F, 0.0F, -1.0F));

		push3(&indices, 1, 4, 8);//Front
		push3(&indices, 5, 1, 8);
		normals.push_back(vector3(0.0F, 0.0F, 1.0F));
		normals.push_back(vector3(0.0F, 0.0F, 1.0F));
		normals.push_back(vector3(0.0F, 0.0F, 1.0F));

		push3(&indices, 1, 5, 2);//Right
		push3(&indices, 5, 6, 2);
		normals.push_back(vector3(1.0F, 0.0F, 0.0F));
		normals.push_back(vector3(1.0F, 0.0F, 0.0F));
		normals.push_back(vector3(1.0F, 0.0F, 0.0F));

		push3(&indices, 3, 7, 4);//Left
		push3(&indices, 7, 8, 4);
		normals.push_back(vector3(-1.0F, 0.0F, 0.0F));
		normals.push_back(vector3(-1.0F, 0.0F, 0.0F));
		normals.push_back(vector3(-1.0F, 0.0F, 0.0F));

		ComponentHandle<MeshComponent> componentMesh1 = entity1.component<MeshComponent>();
		componentMesh1->mesh.addVertices(vertices, colors, indices);
	}
	lastButton = Button;

	ComponentHandle<CameraLock> componentCameraSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentCameraSearch))
	{
		ComponentHandle<Transform> transform = entity.component<Transform>();
		renderingManager->camera.setCameraTransform(transform->position, transform->orientation);
	}

	//#2 update Input
	//Update Key Bindings

	//#3 game logic updates
	entitySystem.systems.update_all(timeStep);

	//#4 audio update

	//#5 rendering
	renderingManager->update(entitySystem, timeStep);
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
	for (Entity entity : entitySystem.entities.entities_for_debugging())
	{
		entity.destroy();
	}

	delete renderingManager;
	delete physicsWorld;
}

UVSG* UVSG::getInstance()
{
	return instance;
}