#include "Component.hpp"
#include "World/Entity.hpp"

Component::Component()
{

}

void Component::update(double deltaTime)
{

};

Entity* Component::getParent()
{
	return m_parent;
};

void Component::setParent(Entity* parent)
{
	m_parent = parent;
};

Component::~Component()
{

}