#include "RenderingManager.hpp"
#include <iostream>

RenderingManager::RenderingManager()
{
	string Title = "UVSG";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
	window->setBufferClearColor(0.0F, 0.0F, 1.0F, 1.0F);

	camera = Camera();
	camera.moveCameraPos(vector3(0.0, 10.0F, -25.0F));
	//camera.rotateCamera(camera.getRight(), -5.0F / 57.2957795F);


	AttributeLocation attributes[] = { { 0, "in_Position" }, { 1, "in_Color" } };

	basicShader = ShaderProgram("basicVertex.vs", "basicFragment.fs", attributes, 2);

	// Get a handle for our "Matrix" uniform
	uniform_MVP_ID = glGetUniformLocation(basicShader.programID, "MVP");
}

void RenderingManager::update(entityx::EntityX &entitySystem, float timeStep)
{
	int width, height;
	window->getWindowSize(width, height);

	matrix4 viewMatrix = camera.getViewMatrix();

	matrix4 projectionMatrix = camera.getProjectionMatrix(width, height);
	matrix4 modelMatrix = matrix4();
	matrix4 MVP = matrix4();

	int count = 0;

	window->clearBuffer();

	entityx::ComponentHandle<MeshComponent> componentMeshSearch;
	for (entityx::Entity entity : entitySystem.entities.entities_with_components(componentMeshSearch))
	{
		entityx::ComponentHandle<MeshComponent> componentMesh = entity.component<MeshComponent>();
		modelMatrix = createModelMatrix(entity);
		
		
		MVP = projectionMatrix * viewMatrix * modelMatrix;
		
		basicShader.setActiveProgram();
		glUniformMatrix4fv(uniform_MVP_ID, 1, GL_FALSE, &MVP[0][0]);

		componentMesh->mesh.draw();

		basicShader.deactivateProgram();

		count++;
	}


	entityx::ComponentHandle<VoxelComponent> componentVoxelSearch;
	for (entityx::Entity entity : entitySystem.entities.entities_with_components(componentVoxelSearch))
	{
		entityx::ComponentHandle<VoxelComponent> componentVoxel = entity.component<VoxelComponent>();
		modelMatrix = createModelMatrix(entity);


		MVP = projectionMatrix * viewMatrix * modelMatrix;

		basicShader.setActiveProgram();
		glUniformMatrix4fv(uniform_MVP_ID, 1, GL_FALSE, &MVP[0][0]);

		componentVoxel->draw();

		basicShader.deactivateProgram();

		count++;
	}


	window->updateBuffer();
}

matrix4	RenderingManager::createModelMatrix(entityx::Entity entity)
{
	matrix4 PositionMatrix = matrix4();
	matrix4 RotationMatrix = matrix4();
	matrix4 ScaleMatrix = matrix4();

	if (entity.has_component<Transform>())
	{
		entityx::ComponentHandle<Transform> componentTransform = entity.component<Transform>();

		 PositionMatrix = glm::translate(matrix4(1.0F), componentTransform->position);
		 RotationMatrix = glm::toMat4(componentTransform->orientation);
	}

	return PositionMatrix * RotationMatrix * ScaleMatrix;
}


RenderingManager::~RenderingManager()
{
	basicShader.deleteProgram();
	window->closeWindow();
	delete window;
}