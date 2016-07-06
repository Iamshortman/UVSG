#include "World/World.hpp"
#include "World/EntityManager.hpp"
#include "Rendering/Camera.hpp"
#include "UVSG.hpp"

#include "Renderer/EntityRenderer.hpp"
#include "Renderer/PlanetRenderer.hpp"

#include "Rendering/RenderingManager.hpp"

World::World()
{
	m_physicsWorld = new PhysicsWorld();
	m_renderer = new EntityRenderer();
	m_PlanetRenderer = new PlanetRenderer();
}

void World::updateWorld(double deltaTime)
{
	m_physicsWorld->update(deltaTime);

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
				EntityManager::Instance->destroyEntity(entity->m_entityId);
			}
		}
	}
}

void World::render(Camera* camera)
{
	vector<Entity*> entities;
	getEntitiesInWorld(entities);

	for (Entity* entity : entities)
	{
		if (entity != nullptr)
		{
			if (entity->m_renderer == nullptr)
			{
				m_renderer->renderAmbient(this, entity, camera);
			}
			else
			{
				entity->m_renderer->renderAmbient(this, entity, camera);
			}
		}
	}

	//Transparent Pass
	for (Entity* entity : entities)
	{
		if (entity != nullptr)
		{
			if (entity->m_renderer == nullptr)
			{
				m_renderer->renderTransparency(this, entity, camera);
			}
			else
			{
				entity->m_renderer->renderTransparency(this, entity, camera);
			}
		}
	}
}

void World::renderFarView(Camera* camera)
{
	RenderingManager* manager = UVSG::getInstance()->renderingManager;

	vector<Entity*> entities;
	getEntitiesInWorld(entities);

	matrix4 projectionMatrix = camera->getProjectionMatrix();
	matrix4 viewMatrix = camera->getOriginViewMatrix();

	for (Entity* entity : entities)
	{
		if (entity->hasComponent("planet"))
		{
			m_PlanetRenderer->renderAmbient(this, entity, camera);
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
		entities.push_back(EntityManager::Instance->getEntity(entityId));
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
	delete m_renderer;
}