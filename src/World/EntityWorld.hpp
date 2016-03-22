#ifndef ENTITYWORLD_HPP
#define ENTITYWORLD_HPP

#include "World/World.hpp"
#include "World/Entity.hpp"

//One of the few times I will ever use multiple inheritance.
//This object both is an Entity and contains its own sub world inside of it.
//An example of this is a spaceship(Entity) and its interior(Sub World). 
class EntityWorld : public World, public Entity
{
public:
	EntityWorld(EntityId id);
	virtual Transform getWorldOffsetMatrix();
};

#endif //ENTITYWORLD_HPP