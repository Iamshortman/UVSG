#include "World/World.hpp"
#include "World/EntityManager.hpp"
#include "Rendering/Camera.hpp"
#include "UVSG.hpp"

#include "Rendering/EntityRenderer.hpp"

World::World()
{
	m_physicsWorld = new PhysicsWorld();
	m_renderer = new EntityRenderer();
}

void World::updateWorld(double deltaTime)
{
	vector<Entity*> entities;
	getEntitiesInWorld(entities);

	for (Entity* entity : entities)
	{
		if (entity != nullptr)
		{
			if (entity->isAlive())
			{
				entity->update(deltaTime);
			}
			else
			{
				EntityManager::instance()->destroyEntity(entity->m_entityId);
			}
		}
	}
	m_physicsWorld->update(deltaTime);
}

void World::render(Camera* camera)
{
	vector<Entity*> entities;
	getEntitiesInWorld(entities);

	for (Entity* entity : entities)
	{
		if (entity != nullptr)
		{
			((EntityRenderer*)m_renderer)->renderAmbient(this, entity, camera);
		}
	}
}
void World::renderFarView(Camera* camera)
{
	const double farViewScaleValue = 100000000.0;

	vector<Entity*> entities;
	getEntitiesInWorld(entities);

	matrix4 projectionMatrix = camera->getProjectionMatrix();
	matrix4 viewMatrix = camera->getOriginViewMatrix();

	for (Entity* entity : entities)
	{
		if (entity->hasComponent("planet"))
		{
			Model* model = (Model*)entity->tempModels[0];
			Transform entityTransform = entity->getTransform();

			if (model != nullptr)
			{
				matrix4 projectionMatrix = camera->getProjectionMatrix();


				matrix4 viewMatrix = camera->getOriginViewMatrix();

				matrix4 modelMatrix = entityTransform.getModleMatrix(camera->getPos(), farViewScaleValue);
				matrix3 normalMatrix = entityTransform.getNormalMatrix();

				modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
				normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();

				model->shader->setActiveProgram();

				model->shader->setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);
				model->shader->setUniform("normalMatrix", normalMatrix);
				model->shader->setUniform("ambientLight", UVSG::getInstance()->renderingManager->ambientLight);

				if (model->mesh != nullptr)
				{
					model->mesh->draw(model->shader);
				}

				model->shader->deactivateProgram();
			}
		}
	}
}

void World::addEntityToWorld(EntityId entityId)
{
	if (m_entitiesInWorld.find(entityId) == m_entitiesInWorld.end())
	{
		m_entitiesInWorld.insert(entityId);
	}
}

void World::removeEntityFromWorld(EntityId entityId)
{
	if (m_entitiesInWorld.find(entityId) != m_entitiesInWorld.end())
	{
		m_entitiesInWorld.erase(entityId);
	}
}

void World::getEntitiesInWorld(vector<Entity*> &entities)
{
	for (set<EntityId>::iterator i = m_entitiesInWorld.begin(); i != m_entitiesInWorld.end(); i++)
	{
		EntityId entityId = *i;
		entities.push_back(EntityManager::instance()->getEntity(entityId));
	}
}

vector3D World::getGravity()
{
	return toGlmVec3(m_physicsWorld->dynamicsWorld->getGravity());
}

void World::setGravity(vector3D gravity)
{
	m_physicsWorld->dynamicsWorld->setGravity(toBtVec3(gravity));
}

Transform World::getWorldOffsetMatrix()
{
	Transform transform = Transform();
	return transform;
}

World::~World()
{
	delete m_physicsWorld;
}