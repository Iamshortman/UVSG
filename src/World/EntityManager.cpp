#include "World/EntityManager.hpp"

EntityManager* EntityManager::m_instance = nullptr;

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

vector<Entity*> EntityManager::getAllEntities()
{
	vector<Entity*> entities;

	for (auto entity : m_entities)
	{
		entities.push_back(entity.second);
	}

	return entities;
}

void EntityManager::createInstance()
{
	m_instance = new EntityManager();
}

EntityManager* EntityManager::instance()
{
	return m_instance;
}