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
}

void RenderingManager::update(EntityX &entitySystem, double timeStep)
{
	int width, height;
	window->getWindowSize(width, height);

	matrix4 viewMatrix = camera.getViewMatrix();

	camera.setProjection(45.0f, 0.001f, 1000.0f, width, height);
	matrix4 projectionMatrix = camera.getProjectionMatrix();

	window->clearBuffer();

	//Far object Rendering Start
	/***************************************************************/

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

		matrix4 m_positionMatrix = matrix4();
		matrix4 m_scaleMatrix = matrix4();
		m_positionMatrix = glm::translate(matrix4(1.0F), floatPos);
		m_scaleMatrix = glm::scale(matrix4(1.0F), floatScale);
		matrix4 modModelMatrix = m_positionMatrix * m_scaleMatrix;

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
			model->shader->setUniform("localOffset", matrix4(1.0f));
			model->mesh->draw();

			model->shader->deactivateProgram();
		}
	}

	//Clear depth buffer so any other object in front of far objects.
	glClear(GL_DEPTH_BUFFER_BIT);

	/*****************************************************************/
	//Far object Rendering End

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

		matrix4 m_positionMatrix = matrix4();
		matrix4 m_scaleMatrix = matrix4();
		m_positionMatrix = glm::translate(matrix4(1.0F), floatPos);
		m_scaleMatrix = glm::scale(matrix4(1.0F), floatScale);
		matrix4 modModelMatrix = m_positionMatrix * m_scaleMatrix;

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
			model->shader->setUniform("localOffset", matrix4(1.0f));

			model->mesh->draw();

			model->shader->deactivateProgram();
		}
	}

	UVSG::getInstance()->editor.TempRender(camera, texturePool);

	window->updateBuffer();
}

RenderingManager::~RenderingManager()
{
	window->closeWindow();
	delete window;
}