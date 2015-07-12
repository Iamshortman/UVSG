#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "RenderingManager.hpp"
#include "Components.hpp"
#include "VoxelCollisionShape.hpp"

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
	this->physicsWorld = new PhysicsWorld();


	entityx::Entity entity = entitySystem.entities.create();
	entity.assign<MeshComponent>();

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

	entityx::ComponentHandle<MeshComponent> componentMesh = entity.component<MeshComponent>();
	componentMesh->mesh.addVertices(vertices1, colors1, normals1, indices1);
	componentMesh->offset = vector3(0.0f, 0.0f, 0.0f);
	entity.assign<RigidBody>(physicsWorld, entity, new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0), 0.0f);


	for (int i = 0; i < 1; i++)
	{
		entityx::Entity entity1 = entitySystem.entities.create();
		entity1.assign<MeshComponent>();
		entity1.assign<Transform>();
		entity1.assign<Velocity>();
		entity1.assign<inputControl>();
		entity1.assign<RigidBody>(physicsWorld, entity1, new VoxelCollisionShape(), 1.0f);

		entity1.component<Transform>()->position = vector3(0.0f, 40.0f, 0.0f);
		entity1.component<Transform>()->orientation = quaternion(1.0f, 0.0f, 0.0f, 0.0f);

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

		entityx::ComponentHandle<MeshComponent> componentMesh1 = entity1.component<MeshComponent>();
		componentMesh1->mesh.addVertices(vertices, colors, indices);
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
	int Button = SDL_JoystickGetButton(SDL_JoystickOpen(0), 1);

	if (Button && !lastButton)
	{

		entityx::Entity entity1 = entitySystem.entities.create();
		entity1.assign<MeshComponent>();
		entity1.assign<Transform>();
		entity1.assign<Velocity>();
		entity1.assign<RigidBody>(physicsWorld, entity1, new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)), 10.0f);
		//entity1.assign<CameraLock>();

		entity1.component<Transform>()->position = renderingManager->camera.getForward() + renderingManager->camera.getPos();
		entity1.component<Velocity>()->linearVelocity = renderingManager->camera.getForward() * 200.0f;

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

		entityx::ComponentHandle<MeshComponent> componentMesh1 = entity1.component<MeshComponent>();
		componentMesh1->mesh.addVertices(vertices, colors, indices);
	}
	lastButton = Button;

	entityx::ComponentHandle<CameraLock> componentCameraSearch;
	for (entityx::Entity entity : entitySystem.entities.entities_with_components(componentCameraSearch))
	{
		entityx::ComponentHandle<Transform> transform = entity.component<Transform>();
		renderingManager->camera.setCameraTransform(transform->position, transform->orientation);
	}

	//#2 update Input
	//Update Key Bindings

	//#3 game logic updates

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
	delete renderingManager;
	delete physicsWorld;
}

UVSG* UVSG::getInstance()
{
	return instance;
}