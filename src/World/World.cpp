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

	entities.clear();
	getEntitiesInWorld(entities);

	m_physicsWorld->update(deltaTime, entities);
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

Transform World::getWorldOffsetMatrix()
{
	Transform transform = Transform();
	return transform;
}

World::~World()
{
	delete m_physicsWorld;
}