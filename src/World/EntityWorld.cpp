#include "World/EntityWorld.hpp"

EntityWorld::EntityWorld(EntityId id)
	:Entity(id), World()
{
	//Nothing I really need to do here
}

void EntityWorld::update(double deltaTime)
{
	this->updateWorld(deltaTime);

	Entity::update(deltaTime);
}

Transform EntityWorld::getWorldOffsetMatrix()
{
	return this->getTransform();
}