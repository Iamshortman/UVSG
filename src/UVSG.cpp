#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "RenderingManager.hpp"
#include "Components.hpp"

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

	entityx::Entity entity1 = entitySystem.entities.create();
	entity1.assign<MeshComponent>();

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
	componentMesh1->offset = vector3(0.0f, 2.0f, 0.0f);
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
		//renderingManager.window->HandleEvent(event);
	}


	//#1 step Physics
	//dynamicsWorld->stepSimulation(timeStep, 4);

	//#2 update Input
	//Update Key Bindings

	//#3 game logic updates

	//#4 audio update

	//#5 rendering
	renderingManager.update(entitySystem, timeStep);
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

}

UVSG* UVSG::getInstance()
{
	return instance;
}