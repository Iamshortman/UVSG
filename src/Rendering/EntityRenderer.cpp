#include "Rendering/EntityRenderer.hpp"
#include "ObjLoader.hpp"

#include "UVSG.hpp"

EntityRenderer::EntityRenderer()
{

}

EntityRenderer::~EntityRenderer()
{

}

void EntityRenderer::renderAmbient(World* world, Entity* entity, Camera* camera)
{
	vector3F ambientLight = UVSG::getInstance()->renderingManager->ambientLight;

	Model* model = nullptr;

	Transform entityTransform = entity->m_transform;

	for (int i = 0; i < entity->tempModels.size(); i++)
	{
		model = (Model*) entity->tempModels[i];

		matrix4 projectionMatrix = camera->getProjectionMatrix();
		matrix4 viewMatrix = camera->getOriginViewMatrix();

		matrix4 modelMatrix = entityTransform.getModleMatrix(camera->getPos());
		matrix3 normalMatrix = entityTransform.getNormalMatrix();

		modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
		normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();

		model->shader->setActiveProgram();

		model->shader->setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);
		model->shader->setUniform("normalMatrix", normalMatrix);
		model->shader->setUniform("ambientLight", ambientLight);

		if (model->mesh != nullptr)
		{
			model->mesh->draw(model->shader);
		}

		model->shader->deactivateProgram();
	}

}

void EntityRenderer::renderLighting(World* world, Entity* entity, Camera* camera)
{

}