#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <vector>
#include <hash_map>
#include <algorithm>
#include "World/Entity.hpp"

class EntityManager
{

public:
	//Instance for the Singleton design pattern;
	static EntityManager* Instance;

	EntityManager();
	Entity* createNewEntity();
	void destroyEntity(EntityId id);

	Entity* getEntity(EntityId id);
	std::vector<Entity*> getAllEntities();

private:
	std::hash_map<EntityId, Entity*> m_entities;
	EntityId nextId = 1;

};

#endif //ENTITYMANAGER_HPP