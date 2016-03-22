#include "World/EntityWorld.hpp"

EntityWorld::EntityWorld(EntityId id)
	:Entity(id), World()
{
	//Nothing I really need to do here
}

Transform EntityWorld::getWorldOffsetMatrix()
{
	Transform transform = Transform();
	transform.setPositionAndRotationFromTransform(this->m_transform);
	return transform;
}