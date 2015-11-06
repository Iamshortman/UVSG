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

	texturedLightShader = ShaderProgram("TexturedLightVertex.vs", "TexturedLightFragment.fs", attributes1, 3);

	texturePool.loadTexture("stone.png");
	texturePool.loadTexture("arrow-up.png");

	bool failed = texturePool.bindTexture("stone.png");
	failed = !failed;

	int chunkSize = chunk.chunkSize;

	for (unsigned int x = 0; x < chunkSize; x++)
	{
		for (unsigned int y = 0; y < chunkSize; y++)
		{
			for (unsigned int z = 0; z < chunkSize; z++)
			{
				if ( (x + y + z) % 2 == 0 )
				{
					chunk.setBlock(x, y, z, 1);
				}
				else
				{
					chunk.setBlock(x, y, z, 1);
				}
			}
		}
	}

	chunk.updateChunk();
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
		basicShader.setUniform("MVP", MVP);
		basicShader.setUniform("offset", componentMesh->offset);
		basicShader.setUniform("normalMatrix", normalMatrix);

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
		texturedShader.setUniform("MVP", MVP);
		texturedShader.setUniform("normalMatrix", normalMatrix);

		texturePool.bindTexture("stone.png");
		componentMesh->draw();

		texturedShader.deactivateProgram();

		count++;
	}

	for (int x = 0; x < 1; x++)
	{
		for (int z = 0; z < 2; z++)
		{
			//Chunk render
			Transform transform;
			transform.position += vector3(x * 16, 10, z * 16);
			modelMatrix = transform.getModleMatrix();
			normalMatrix = transform.getNormalMatrix();
			MVP = projectionMatrix * viewMatrix * modelMatrix;

			texturedLightShader.setActiveProgram();
			texturedLightShader.setUniform("MVP", MVP);
			texturedLightShader.setUniform("normalMatrix", normalMatrix);
			texturedLightShader.setUniform("modelMatrix", modelMatrix);
			texturedLightShader.setUniform("lightColor", vector3(1.0f));
			texturedLightShader.setUniform("lightPos", camera.getPos());


			texturePool.bindTexture("stone.png");
			chunk.render();

			texturedLightShader.deactivateProgram();

		}
	}



	window->updateBuffer();
}

RenderingManager::~RenderingManager()
{
	basicShader.deleteProgram();
	window->closeWindow();
	delete window;
}