#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <vector>
#include <hash_map>
#include <algorithm>
#include "World/Entity.hpp"

class EntityManager
{
private:
	static EntityManager* m_instance;

	std::hash_map<EntityId, Entity*> m_entities;
	EntityId nextId = 1;

public:
	EntityManager();
	Entity* createNewEntity();
	void destroyEntity(EntityId id);

	Entity* getEntity(EntityId id);
	vector<Entity*> getAllEntities();

	static void createInstance();
	static EntityManager* instance();
};

#endif //ENTITYMANAGER_HPP