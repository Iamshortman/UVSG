#include "RenderingManager.hpp"

#include <iostream>
#include "Renderable.hpp"
#include "UVSG.hpp"

RenderingManager::RenderingManager()
{
	string Title = "UVSG";
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 400;

	window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, Title);
	window->setBufferClearColor(0.0F, 0.0F, 0.0F, 1.0F);

	camera = Camera();

	texturePool.loadTexture("res/stone.png");
	texturePool.loadTexture("res/arrow-up.png");
	texturePool.loadTexture("res/StarRed.png");
	texturePool.loadTexture("res/ShipParts/Laser_Cannon.png");
}

void RenderingManager::update(EntityX &entitySystem, double timeStep)
{
	int width, height;
	window->getWindowSize(width, height);

	//matrix4 viewMatrix = camera.getViewMatrix();

	window->clearBuffer();

	//Far object Rendering Start
	/***************************************************************/

	camera.setProjection(45.0f, 0.1f, 10000.0f, width, height);
	matrix4 projectionMatrix = camera.getProjectionMatrix();

	ComponentHandle<Transform> componentTransformSearch;
	ComponentHandle<FarZoneRenderable> componentFarZoneRenderableSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentFarZoneRenderableSearch, componentTransformSearch))
	{
		ComponentHandle<FarZoneRenderable> componentRenderable = entity.component<FarZoneRenderable>();
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();

		vector3D camPos = camera.getPos();
		vector3D offsetPos = componentTransform->getPos() - camPos;
		vector3D scale = componentTransform->getScale();
		offsetPos /= farViewScaleValue;
		scale /= farViewScaleValue;

		vector3F floatPos = (vector3F)offsetPos;
		vector3F floatScale = (vector3F)scale;
		quaternionF floatOrientation = (quaternionF)componentTransform->getOrientation();

		matrix4 positionMatrix = matrix4();
		matrix4 scaleMatrix = matrix4();
		matrix4 rotationMatrix = matrix4();

		rotationMatrix = glm::toMat4(floatOrientation);
		positionMatrix = glm::translate(matrix4(1.0F), floatPos);
		scaleMatrix = glm::scale(matrix4(1.0F), floatScale);
		matrix4 modModelMatrix = positionMatrix * rotationMatrix * scaleMatrix;

		for (int i = 0; i < componentRenderable->models.size(); i++)
		{
			Model* model = componentRenderable->models[i];
			model->shader->setActiveProgram();
			if (model->texture != "")
			{
				texturePool.bindTexture(model->texture);
			}

			model->shader->setUniform("MVP", projectionMatrix * camera.getOriginViewMatrix() * modModelMatrix);
			model->shader->setUniform("normalMatrix", componentTransform->getNormalMatrix());
			model->shader->setUniform("localOffset", model->localOffset.getModleMatrix());
			model->mesh->draw(model->shader);

			model->shader->deactivateProgram();
		}

        matrix4 modelViewMatrix = camera.getOriginViewMatrix() * modModelMatrix;

        //Clear rotations
        // Column 0:
        modelViewMatrix[0][0] = 1;
        modelViewMatrix[0][1] = 0;
        modelViewMatrix[0][2] = 0;

        // Column 1:
        modelViewMatrix[1][0] = 0;
        modelViewMatrix[1][1] = 1;
        modelViewMatrix[1][2] = 0;

        // Column 2:
        modelViewMatrix[2][0] = 0;
        modelViewMatrix[2][1] = 0;
        modelViewMatrix[2][2] = 1;

        matrix4 billboardMatrix = projectionMatrix * modelViewMatrix;

        for (int i = 0; i < componentRenderable->billboards.size(); i++)
		{
			Billboard* billboard = componentRenderable->billboards[i];
			billboard->shader->setActiveProgram();

            billboard->shader->setUniform("tempColor", billboard->color);
            billboard->shader->setUniform("MVP", billboardMatrix);
            billboard->shader->setUniform("scale", billboard->scale * floatScale);

			billboard->mesh->draw(billboard->shader);

			billboard->shader->deactivateProgram();
        }
	}

	//Clear depth buffer so any other object in front of far objects.
	glClear(GL_DEPTH_BUFFER_BIT);

	/*****************************************************************/
	//Far object Rendering End


	/*****************************************************************/
	//Near object Rendering Start

	camera.setProjection(45.0f, 0.001f, 1000.0f, width, height);
	projectionMatrix = camera.getProjectionMatrix();

	ComponentHandle<NearZoneRenderable> componentNearZoneRenderableSearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentNearZoneRenderableSearch, componentTransformSearch))
	{
		ComponentHandle<NearZoneRenderable> componentRenderable = entity.component<NearZoneRenderable>();
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();

		vector3D camPos = camera.getPos();
		vector3D offsetPos = componentTransform->getPos() - camPos;
		vector3D scale = componentTransform->getScale();

		vector3F floatPos = (vector3F)offsetPos;
		vector3F floatScale = (vector3F)scale;
		quaternionF floatOrientation = (quaternionF)componentTransform->getOrientation();

		matrix4 positionMatrix = matrix4();
		matrix4 scaleMatrix = matrix4();
		matrix4 rotationMatrix = matrix4();

		rotationMatrix = glm::toMat4(floatOrientation);
		positionMatrix = glm::translate(matrix4(1.0F), floatPos);
		scaleMatrix = glm::scale(matrix4(1.0F), floatScale);
		matrix4 modModelMatrix = positionMatrix * rotationMatrix * scaleMatrix;

		for (int i = 0; i < componentRenderable->models.size(); i++)
		{
			Model* model = componentRenderable->models[i];
			model->shader->setActiveProgram();
			if (model->texture != "")
			{
				texturePool.bindTexture(model->texture);
			}

			model->shader->setUniform("MVP", projectionMatrix * camera.getOriginViewMatrix() * modModelMatrix);
			model->shader->setUniform("normalMatrix", componentTransform->getNormalMatrix());
			model->shader->setUniform("localOffset", model->localOffset.getModleMatrix());

			model->mesh->draw(model->shader);

			model->shader->deactivateProgram();
		}
	}
	/*****************************************************************/
	//Near object Rendering End

	window->updateBuffer();
}

RenderingManager::~RenderingManager()
{
	window->closeWindow();
	delete window;
}
