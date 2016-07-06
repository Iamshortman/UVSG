#include "World/EntityManager.hpp"

EntityManager* EntityManager::Instance = nullptr;

EntityManager::EntityManager()
{

}

Entity* EntityManager::createNewEntity()
{
	Entity* entity = new Entity(nextId);
	m_entities[nextId] = entity;
	nextId++;
	return entity;
}
void EntityManager::destroyEntity(EntityId id)
{
	if (m_entities.count(id))
	{
		delete m_entities[id];
		m_entities.erase(id);
	}
}

Entity* EntityManager::getEntity(EntityId id)
{
	return m_entities[id];
}

std::vector<Entity*> EntityManager::getAllEntities()
{
	std::vector<Entity*> entities;

	for (auto entity : m_entities)
	{
		entities.push_back(entity.second);
	}

	return entities;
}