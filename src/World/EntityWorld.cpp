#include "World/EntityWorld.hpp"

EntityWorld::EntityWorld(EntityId id)
	:Entity(id), World()
{
	//Nothing I really need to do here
}

Transform EntityWorld::getWorldOffsetMatrix()
{
	return this->getTransform();
}