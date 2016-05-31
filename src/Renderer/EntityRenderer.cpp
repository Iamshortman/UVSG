#include "Renderer/EntityRenderer.hpp"
#include "Rendering/RenderingManager.hpp"

EntityRenderer::EntityRenderer()
{
	directionalLight = new DirectionalLight(vector3F(1.0f, -1.0f, 1.0f), vector3F(1.0f, 1.0f, 1.0f), 0.6f);
	DirectionalShader = new ShaderProgram("res/Material.vs", "res/foward-directional.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
}

EntityRenderer::~EntityRenderer()
{
	delete directionalLight;
	delete DirectionalShader;
}

void EntityRenderer::renderAmbient(World* world, Entity* entity, Camera* camera)
{
	vector3F ambientLight = world->ambientLight;

	Model* model = nullptr;

	Transform entityTransform = entity->getTransform();
	Transform worldOffset = world->getWorldOffsetMatrix();

	for (int i = 0; i < entity->tempModels.size(); i++)
	{
		model = (Model*) entity->tempModels[i];

		matrix4 projectionMatrix = camera->getProjectionMatrix();
		matrix4 viewMatrix = camera->getOriginViewMatrix();

		matrix4 modelMatrix = entityTransform.getModleMatrix(camera->getPos());
		matrix3 normalMatrix = entityTransform.getNormalMatrix();

		modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
		normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();

		matrix4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		model->shader->setActiveProgram();

		model->shader->setUniform("MVP", MVP);
		model->shader->setUniform("normalMatrix", normalMatrix);
		model->shader->setUniform("ambientLight", ambientLight);

		if (model->mesh != nullptr)
		{
			model->mesh->draw(model->shader);
		}

		model->shader->deactivateProgram();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(false);
		glDepthFunc(GL_EQUAL);

		DirectionalShader->setActiveProgram();
		DirectionalShader->setUniform("MVP", MVP);
		DirectionalShader->setUniform("normalMatrix", normalMatrix);
		DirectionalShader->setUniform("modelMatrix", modelMatrix);
		DirectionalShader->setUniform("directionalLight.color", directionalLight->getColor());
		DirectionalShader->setUniform("directionalLight.direction", directionalLight->getDirection());
		DirectionalShader->setUniform("directionalLight.intensity", directionalLight->getIntensity());
		model->mesh->draw(DirectionalShader);
		DirectionalShader->deactivateProgram();


		glDepthFunc(GL_LESS);
		glDepthMask(true);
		glDisable(GL_BLEND);
	}

}

void EntityRenderer::renderLighting(World* world, Entity* entity, Camera* camera)
{

}

RenderView EntityRenderer::getRenderView()
{
	return NearView;
}