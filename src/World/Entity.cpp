#include "World/Entity.hpp"
#include "World/World.hpp"

Entity::Entity(EntityId id)
	:m_entityId(id)
{

}

void Entity::update(double deltaTime)
{
	//Not a huge fan of the auto keyword
	//But what are you going to do
	for (auto iterator : m_components)
	{
		iterator.second->update(deltaTime);
	}
}

void Entity::addToWorld(World* world)
{
	//Remove from the current world
	if (m_world != nullptr)
	{
		m_world->removeEntityFromWorld(m_entityId);
	}

	//Set the new world even if its null
	m_world = world;

	//add to the current world
	if (m_world != nullptr)
	{
		m_world->addEntityToWorld(m_entityId);
	}

	if (m_RigidBody != nullptr)
	{
		m_RigidBody->addToPhysicsWorld(m_world->m_physicsWorld, this);
	}
}

World* Entity::getWorld()
{
	return m_world;
}

void Entity::addRigidBody(RigidBody* rigidBody)
{
	m_RigidBody = rigidBody;
	if (m_world != nullptr)
	{
		m_RigidBody->addToPhysicsWorld(m_world->m_physicsWorld, this);
	}
}

bool Entity::hasComponent(string componentName)
{
	return m_components.find(componentName) != m_components.end();
}

Component* Entity::getComponent(string componentName)
{
	return m_components[componentName];
}

void Entity::addComponent(string componentName, Component* component)
{
	m_components[componentName] = component;
	component->setParent(this);
}

void Entity::removeComponent(string componentName)
{
	delete m_components[componentName];
	m_components.erase(componentName);
}

void Entity::kill()
{
	this->m_alive = false;
}

Entity::~Entity()
{
	delete m_RigidBody;

	//Remove from the current world
	if (m_world != nullptr)
	{
		m_world->removeEntityFromWorld(m_entityId);
	}

	//Not a huge fan of the auto keyword
	//But what are you going to do
	for (auto iterator : m_components)
	{
		delete iterator.second;
	}
}