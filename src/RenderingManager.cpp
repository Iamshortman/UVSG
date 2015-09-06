#include "RenderingManager.hpp"

#include <iostream>

RenderingManager::RenderingManager()
{
	string Title = "UVSG";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
	window->setBufferClearColor(0.0F, 0.0F, 0.0F, 1.0F);

	camera = Camera();

	AttributeLocation attributes[] = { { 0, "in_Position" }, { 1, "in_Color" }, { 2, "in_Normal" } }; 
	basicShader = ShaderProgram("basicVertex.vs", "basicFragment.fs", attributes, 3);

	AttributeLocation attributes1[] = { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } };
	texturedShader = ShaderProgram("TexturedVertex.vs", "TexturedFragment.fs", attributes1, 3);

	// Get a handle for our "Matrix" uniform
	uniform_MVP_ID = glGetUniformLocation(basicShader.programID, "MVP");
	uniform_Offset_ID = glGetUniformLocation(basicShader.programID, "offset");
	uniform_Normal_ID = glGetUniformLocation(basicShader.programID, "normalMatrix");

	uniform_MVP_ID1 = glGetUniformLocation(texturedShader.programID, "MVP");
	uniform_Normal_ID1 = glGetUniformLocation(texturedShader.programID, "normalMatrix");

	texturePool.loadTexture("stone.png");
	bool failed = texturePool.bindTexture("stone.png");

	failed = !failed;
}

void RenderingManager::update(EntityX &entitySystem, double timeStep)
{
	int width, height;
	window->getWindowSize(width, height);

	matrix4 viewMatrix = camera.getViewMatrix();

	matrix4 projectionMatrix = camera.getProjectionMatrix(width, height);
	matrix4 modelMatrix = matrix4();
	matrix4 MVP = matrix4();
	matrix3 normalMatrix = matrix3();

	int count = 0;

	window->clearBuffer();

	ComponentHandle<MeshComponent> componentMeshSearch;
	ComponentHandle<Transform> componentTransformSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentMeshSearch, componentTransformSearch))
	{
		ComponentHandle<MeshComponent> componentMesh = entity.component<MeshComponent>();
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();

		modelMatrix = componentTransform->getModleMatrix();
		normalMatrix = componentTransform->getNormalMatrix();

		MVP = projectionMatrix * viewMatrix * modelMatrix;
		
		basicShader.setActiveProgram();
		glUniformMatrix4fv(uniform_MVP_ID, 1, GL_FALSE, &MVP[0][0]);
		glUniform3fv(uniform_Offset_ID, 1, &componentMesh->offset[0]);
		glUniformMatrix3fv(uniform_Normal_ID, 1, GL_FALSE, &normalMatrix[0][0]);

		componentMesh->mesh.draw();

		basicShader.deactivateProgram();

		count++;
	}

	ComponentHandle<TexturedMesh> componentTexturedMeshSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentTexturedMeshSearch, componentTransformSearch))
	{
		ComponentHandle<TexturedMesh> componentMesh = entity.component<TexturedMesh>();
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();

		modelMatrix = componentTransform->getModleMatrix();
		normalMatrix = componentTransform->getNormalMatrix();

		MVP = projectionMatrix * viewMatrix * modelMatrix;

		texturedShader.setActiveProgram();
		glUniformMatrix4fv(uniform_MVP_ID1, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix3fv(uniform_Normal_ID1, 1, GL_FALSE, &normalMatrix[0][0]);

		texturePool.bindTexture("stone.png");
		componentMesh->draw();

		texturedShader.deactivateProgram();

		count++;
	}


	window->updateBuffer();
}

RenderingManager::~RenderingManager()
{
	basicShader.deleteProgram();
	window->closeWindow();
	delete window;
}