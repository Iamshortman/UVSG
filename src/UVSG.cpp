#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "RenderingManager.hpp"
#include "Components.hpp"
#include "TimeToLive.hpp"

#include "PlayerControl.hpp"
#include "ThrusterControl.hpp"
#include "RigidBody.hpp"

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
	this->physicsWorld = new PhysicsWorld();

	entitySystem.systems.add<TimeToLiveSystem>();
	entitySystem.systems.add<PlayerControlSystem>();
	//entitySystem.systems.add<ThrusterControlSystem>();
	entitySystem.systems.configure();

	Entity camEntity = entitySystem.entities.create();
	camEntity.assign<Transform>();
	camEntity.assign<CameraLock>();
	camEntity.assign<PlayerControlComponent>(48.0f, 3.0f);
	camEntity.component<Transform>()->position = vector3(0.0f);

	//Sets the camera to look at the center of the world
	//camEntity.component<Transform>()->orientation = glm::angleAxis(toRad(45.0f), vector3(1, 0, 0)) * camEntity.component<Transform>()->orientation;
	//camEntity.component<Transform>()->orientation = glm::angleAxis(toRad(-135.0f), vector3(0, 1, 0)) * camEntity.component<Transform>()->orientation;


	Entity groundEntity = entitySystem.entities.create();
	groundEntity.assign<Transform>();
	groundEntity.component<Transform>()->position = vector3(0.0f, 0.0f, 0.0f);

	vector<Vertex> verticesStruct = vector<Vertex>();
	Vertex vertex1 = { vector3(-50.0f, 0.0f, 50.0f), vector3(0.0F, 1.0F, 0.0F) , vector2(0.0f, 1.0f)};
	Vertex vertex2 = { vector3(50.0f, 0.0f, 50.0f), vector3(0.0F, 1.0F, 0.0F), vector2(1.0f, 1.0f) };
	Vertex vertex3 = { vector3(-50.0f, 0.0f, -50.0f), vector3(0.0F, 1.0F, 0.0F), vector2(0.0f, 0.0f) };
	Vertex vertex4 = { vector3(50.0f, 0.0f, -50.0f), vector3(0.0F, 1.0F, 0.0F), vector2(1.0f, 0.0f) };
	vector<unsigned int> indicesStruct = {0, 1, 2, 2, 1, 3};
	verticesStruct.push_back(vertex1); verticesStruct.push_back(vertex2); verticesStruct.push_back(vertex3); verticesStruct.push_back(vertex4);
	groundEntity.assign<TexturedMesh>(verticesStruct, indicesStruct);

	Entity entity1 = entitySystem.entities.create();
	entity1.assign<MeshComponent>();
	entity1.assign<Transform>();
	entity1.assign<Velocity>();
	//entity1.assign<RigidBody>(physicsWorld, entity1, new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)), 10.0f);
	entity1.component<Transform>()->setPos(vector3(5.0f, 10.0f, 5.0f));

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

	//ComponentHandle<MeshComponent> componentMesh1 = entity1.component<MeshComponent>();
	//componentMesh1->mesh.addVertices(vertices, colors, indices);


	//renderingManager->chunk.cube = new Mesh();
	//renderingManager->chunk.cube->addVertices(vertices, colors, indices);
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
	physicsWorld->update(entitySystem, timeStep);

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
	//delete physxWorld;
}

UVSG* UVSG::getInstance()
{
	return instance;
}