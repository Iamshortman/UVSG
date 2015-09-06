#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "RenderingManager.hpp"
#include "Components.hpp"
#include "TimeToLive.hpp"
#include "VoxelSystem.hpp"

#include "PlayerControl.hpp"
#include "ThrusterControl.hpp"

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
	this->renderingManager->window->setVsync(1);

	this->physxWorld = new PhysxWorld();

	entitySystem.systems.add<TimeToLiveSystem>();
	entitySystem.systems.add<VoxelSystem>();
	entitySystem.systems.add<PlayerControlSystem>();
	entitySystem.systems.add<ThrusterControlSystem>();
	entitySystem.systems.configure();

	Entity camEntity = entitySystem.entities.create();
	camEntity.assign<Transform>();
	camEntity.assign<CameraLock>();
	camEntity.assign<PlayerControlComponent>(6.0f, 3.0f);
	camEntity.component<Transform>()->position = vector3(40.0f);

	//Sets the camera to look at the center of the world
	camEntity.component<Transform>()->orientation = glm::angleAxis(toRad(45.0f), vector3(1, 0, 0)) * camEntity.component<Transform>()->orientation;
	camEntity.component<Transform>()->orientation = glm::angleAxis(toRad(-135.0f), vector3(0, 1, 0)) * camEntity.component<Transform>()->orientation;


	Entity groundEntity = entitySystem.entities.create();
	groundEntity.assign<Transform>();
	groundEntity.component<Transform>()->position = vector3(0.0f, -0.0f, 0.0f);

	/*vector<vector3> vertices1 = vector<vector3>();
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

	groundEntity.assign<MeshComponent>();
	groundEntity.component<MeshComponent>()->mesh.addVertices(vertices1, colors1, normals1, indices1);*/

	vector<Vertex> verticesStruct = vector<Vertex>();
	Vertex vertex1 = { vector3(-50.0f, 0.0f, 50.0f), vector3(0.0F, 1.0F, 0.0F) , vector2(0.0f, 1.0f)};
	Vertex vertex2 = { vector3(50.0f, 0.0f, 50.0f), vector3(0.0F, 1.0F, 0.0F), vector2(1.0f, 1.0f) };
	Vertex vertex3 = { vector3(-50.0f, 0.0f, -50.0f), vector3(0.0F, 1.0F, 0.0F), vector2(0.0f, 0.0f) };
	Vertex vertex4 = { vector3(50.0f, 0.0f, -50.0f), vector3(0.0F, 1.0F, 0.0F), vector2(1.0f, 0.0f) };
	vector<unsigned int> indicesStruct = {0, 1, 2, 2, 1, 3};
	verticesStruct.push_back(vertex1); verticesStruct.push_back(vertex2); verticesStruct.push_back(vertex3); verticesStruct.push_back(vertex4);
	groundEntity.assign<TexturedMesh>(verticesStruct, indicesStruct);

	Entity voxelObject = entitySystem.entities.create();

	voxelObject.assign<Transform>();
	voxelObject.component<Transform>()->position = vector3(0.0f, 10.0f, 0.0f);
	voxelObject.assign<VoxelComponent>();
	voxelObject.assign<MeshComponent>();

	for (unsigned int i = 0; i < voxelObject.component<VoxelComponent>()->chunkSize; i++)
	{
		for (unsigned int j = 0; j < voxelObject.component<VoxelComponent>()->chunkSize; j++)
		{
			for (unsigned int k = 0; k < voxelObject.component<VoxelComponent>()->chunkSize; k++)
			{
				voxelObject.component<VoxelComponent>()->setBlock(i, j, k, 0);
			}
		}
	}

	voxelObject.component<VoxelComponent>()->setBlock(0, 0, 0, 1);
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
		renderingManager->window->HandleEvent(event);
	}

	//#1 step Physics
	//physicsWorld->update(entitySystem, timeStep);
	physxWorld->update(entitySystem, timeStep);

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
	renderingManager->window->set3dRendering();
	renderingManager->update(entitySystem, timeStep);

	//#6 UI Rendering
	renderingManager->window->set2dRendering();

}

Entity UVSG::getEntityFromId(entityxId id)
{
	return entitySystem.entities.get(entitySystem.entities.create_id(id));
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
	int count = 0;
	for (Entity entity : entitySystem.entities.entities_for_debugging())
	{
		entity.destroy();
		count++;
	}
	printf("Num Entities: %i \n", count);

	delete renderingManager;
	//delete physicsWorld;
	delete physxWorld;
}

UVSG* UVSG::getInstance()
{
	return instance;
}