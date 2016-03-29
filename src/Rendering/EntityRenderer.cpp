#include "Rendering/EntityRenderer.hpp"
#include "ObjLoader.hpp"


EntityRenderer::EntityRenderer()
{
	baseCubeModel = new Model();
	baseCubeModel->localOffset = Transform();
	baseCubeModel->shader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
	baseCubeModel->mesh = loadMaterialMeshFromFile("res/Models/", "Cube.obj");
}

EntityRenderer::~EntityRenderer()
{
	delete baseCubeModel->mesh;
	delete baseCubeModel->shader;
	delete baseCubeModel;
}

void EntityRenderer::renderAmbient(World* world, Entity* entity, Camera* camera)
{
	Model* model = baseCubeModel;

	if (entity->tempModels.size() == 0)
	{
		matrix4 projectionMatrix = camera->getProjectionMatrix();
		matrix4 viewMatrix = camera->getOriginViewMatrix();
		matrix4 modelMatrix = entity->m_transform.getModleMatrix(camera->getPos());
		matrix3 normalMatrix = entity->m_transform.getNormalMatrix();

		model->shader->setActiveProgram();

		model->shader->setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);
		model->shader->setUniform("normalMatrix", normalMatrix);
		model->shader->setUniform("localOffset", model->localOffset.getModleMatrix());
		model->shader->setUniform("ambientLight", vector3F(1.0));

		model->mesh->draw(baseCubeModel->shader);

		model->shader->deactivateProgram();

		return;
	}

	for (int i = 0; i < entity->tempModels.size(); i++)
	{
		model = (Model*) entity->tempModels[i];

		matrix4 projectionMatrix = camera->getProjectionMatrix();
		matrix4 viewMatrix = camera->getOriginViewMatrix();

		matrix4 modelMatrix = entity->m_transform.getModleMatrix(camera->getPos());
		matrix3 normalMatrix = entity->m_transform.getNormalMatrix();

		modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
		normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();

		model->shader->setActiveProgram();

		model->shader->setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);
		model->shader->setUniform("normalMatrix", normalMatrix);
		model->shader->setUniform("ambientLight", vector3F(1.0));

		model->mesh->draw(baseCubeModel->shader);

		model->shader->deactivateProgram();
	}

}

void EntityRenderer::renderLighting(World* world, Entity* entity, Camera* camera)
{

}