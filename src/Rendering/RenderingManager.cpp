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

	ambientLight = vector3F(1.0f);
	light = new DirectionalLight(glm::normalize(vector3F(1.0f, -1.0f, 0.0f)), vector3F(1, 1, 1), 1.0f);
	DirectionalShader = new ShaderProgram("res/foward-directional.vs", "res/foward-directional.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });

	pointLight = new PointLight(vector3D(0, 5, 0), 10.0f, vector3F(1, 0.1f, 0.1f), 0.4f);
	pointShader = new ShaderProgram("res/foward-point.vs", "res/foward-point.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
}

void RenderingManager::update(EntityX &entitySystem, double timeStep)
{
	int width, height;
	window->getWindowSize(width, height);

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
		matrix4 ModelMatrix = positionMatrix * rotationMatrix * scaleMatrix;

		for (int i = 0; i < componentRenderable->models.size(); i++)
		{
			Model* model = componentRenderable->models[i];
			model->shader->setActiveProgram();
			if (model->texture != "")
			{
				texturePool.bindTexture(model->texture);
			}

			model->shader->setUniform("MVP", projectionMatrix * camera.getOriginViewMatrix() * ModelMatrix);
			model->shader->setUniform("normalMatrix", componentTransform->getNormalMatrix());
			model->shader->setUniform("localOffset", model->localOffset.getModleMatrix());
			model->shader->setUniform("ambientLight", ambientLight);

			model->mesh->draw(model->shader);

			model->shader->deactivateProgram();
		}

        matrix4 modelViewMatrix = camera.getOriginViewMatrix() * ModelMatrix;

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
		matrix4 ModelMatrix = positionMatrix * rotationMatrix * scaleMatrix;

		for (int i = 0; i < componentRenderable->models.size(); i++)
		{
			Model* model = componentRenderable->models[i];
			model->shader->setActiveProgram();

			model->shader->setUniform("MVP", projectionMatrix * camera.getOriginViewMatrix() * ModelMatrix);
			model->shader->setUniform("normalMatrix", componentTransform->getNormalMatrix());
			model->shader->setUniform("localOffset", model->localOffset.getModleMatrix());
			model->shader->setUniform("ambientLight", ambientLight);

			model->mesh->draw(model->shader);

			model->shader->deactivateProgram();

			//Lighting Pass Start
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glDepthMask(false);
			glDepthFunc(GL_EQUAL);

			/*DirectionalShader->setActiveProgram();
			DirectionalShader->setUniform("MVP", projectionMatrix * camera.getOriginViewMatrix() * ModelMatrix);
			DirectionalShader->setUniform("normalMatrix", componentTransform->getNormalMatrix());
			DirectionalShader->setUniform("localOffset", model->localOffset.getModleMatrix());

			DirectionalShader->setUniform("directionalLight.color", light->getColor());
			DirectionalShader->setUniform("directionalLight.direction", light->getDirection());
			DirectionalShader->setUniform("directionalLight.intensity", light->getIntensity());

			model->mesh->draw(DirectionalShader);

			DirectionalShader->deactivateProgram();*/

			/*pointShader->setActiveProgram();
			vector3F lightPos = (vector3F)(pointLight->m_position - camPos);

			pointShader->setUniform("MVP", projectionMatrix * camera.getOriginViewMatrix() * ModelMatrix);
			pointShader->setUniform("modelMatrix", ModelMatrix);
			pointShader->setUniform("normalMatrix", componentTransform->getNormalMatrix());
			pointShader->setUniform("localOffset", model->localOffset.getModleMatrix());

			pointShader->setUniform("pointLight.base.color", pointLight->getColor());
			pointShader->setUniform("pointLight.base.intensity", pointLight->getIntensity());
			pointShader->setUniform("pointLight.atten.constant", 0.0f);
			pointShader->setUniform("pointLight.atten.linear", 0.0f);
			pointShader->setUniform("pointLight.atten.exponent", 0.1f);
			pointShader->setUniform("pointLight.position", lightPos);
			pointShader->setUniform("pointLight.range", pointLight->m_range);

			//model->mesh->draw(pointShader);

			pointShader->deactivateProgram();*/

			glDepthFunc(GL_LESS);
			glDepthMask(true);
			glDisable(GL_BLEND);
		}
	}
	/*****************************************************************/
	//Near object Rendering End

	//Clear depth buffer so any other object in front of far objects.
	glClear(GL_DEPTH_BUFFER_BIT);
	camera.setProjection(45.0f, 0.0001f, 10.0f, width, height);
	projectionMatrix = camera.getProjectionMatrix();

	//FirstPerson

	if (true)
	{
		Transform transform;
		transform.setScale(vector3D(.1));
		transform.setPos(vector3D(0.6, -.4, -1));
		transform.setOrientation(glm::angleAxis(toRad(180.0), vector3D(0, 1, 0)));

		Model* model = firstPersonModel;
		model->shader->setActiveProgram();

		model->shader->setUniform("MVP", projectionMatrix * transform.getModleMatrix());
		model->shader->setUniform("normalMatrix", transform.getNormalMatrix());
		model->shader->setUniform("ambientLight", ambientLight);

		model->mesh->draw(model->shader);

		model->shader->deactivateProgram();
	}


	window->updateBuffer();
}

RenderingManager::~RenderingManager()
{
	window->closeWindow();
	delete window;
}
