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

	AttributeLocation attributes[] = { { 0, "in_Position" }, { 1, "in_Color" } };

	basicShader = ShaderProgram("basicVertex.vs", "basicFragment.fs", attributes, 2);

	// Get a handle for our "Matrix" uniform
	uniform_MVP_ID = glGetUniformLocation(basicShader.programID, "MVP");
	uniform_Offset_ID = glGetUniformLocation(basicShader.programID, "offset");
}

void RenderingManager::update(EntityX &entitySystem, float timeStep)
{
	int width, height;
	window->getWindowSize(width, height);

	matrix4 viewMatrix = camera.getViewMatrix();

	matrix4 projectionMatrix = camera.getProjectionMatrix(width, height);
	matrix4 modelMatrix = matrix4();
	matrix4 MVP = matrix4();

	int count = 0;

	window->clearBuffer();

	ComponentHandle<MeshComponent> componentMeshSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentMeshSearch))
	{
		ComponentHandle<MeshComponent> componentMesh = entity.component<MeshComponent>();
		modelMatrix = createModelMatrix(entity);
		
		MVP = projectionMatrix * viewMatrix * modelMatrix;
		
		basicShader.setActiveProgram();
		glUniformMatrix4fv(uniform_MVP_ID, 1, GL_FALSE, &MVP[0][0]);
		glUniform3fv(uniform_Offset_ID, 1, &componentMesh->offset[0]);
		componentMesh->mesh.draw();

		basicShader.deactivateProgram();

		count++;
	}

	window->updateBuffer();
}

matrix4	RenderingManager::createModelMatrix(Entity entity)
{
	matrix4 PositionMatrix = matrix4();
	matrix4 RotationMatrix = matrix4();
	matrix4 ScaleMatrix = matrix4();

	if (entity.has_component<Transform>())
	{
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();

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