#include "World/Entity.hpp"
#include "World/World.hpp"

Entity::Entity(EntityId id)
	:m_entityId(id)
{

}

void Entity::update(double deltaTime)
{
	//Move Entity based on velocity even if there is no RigidBody.
	if (m_RigidBody == nullptr)
	{
		Transform transform = getTransform();

		//Move Entity Based on Velocity
		transform.setPos(transform.getPos() + (getLinearVelocity() * deltaTime));

		//TODO figure out angular Velocity.

		setTransform(transform);
	}
	else
	{
		//If there is a RigidBody, sync the entities values too the Rigid Bodies values
		vector3D scale = m_transform.getScale();
		m_transform = getTransform();
		m_transform.setScale(scale);
		m_velocity.linearVelocity = getLinearVelocity();
		m_velocity.angularVelocity = getAngularVelocity();
	}

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
		m_RigidBody->addToPhysicsWorld(m_world->m_physicsWorld, this, m_transform);
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
		m_RigidBody->addToPhysicsWorld(m_world->m_physicsWorld, this, m_transform);
	}
}

vector3D Entity::getPosition()
{
	if (m_RigidBody != nullptr)
	{
		return m_RigidBody->getWorldTransform().getPos();
	}
	else
	{
		return m_transform.getPos();
	}
}

void Entity::setPosition(vector3D pos)
{
	if (m_RigidBody != nullptr)
	{
		Transform transform = m_RigidBody->getWorldTransform();
		transform.setPos(pos);
		m_RigidBody->setWorldTransform(transform);
	}
	else
	{
		m_transform.setPos(pos);
	}
}

quaternionD Entity::getOrientation()
{
	if (m_RigidBody != nullptr)
	{
		return m_RigidBody->getWorldTransform().getOrientation();
	}
	else
	{
		return m_transform.getOrientation();
	}
}

void Entity::setOrientation(quaternionD rot)
{
	if (m_RigidBody != nullptr)
	{
		Transform transform = m_RigidBody->getWorldTransform();
		transform.setOrientation(rot);
		m_RigidBody->setWorldTransform(transform);
	}
	else
	{
		m_transform.setOrientation(rot);
	}
}

vector3D Entity::getScale()
{
	return m_transform.getScale();
}

void Entity::setScale(vector3D scale)
{
	m_transform.setScale(scale);
}

Transform Entity::getTransform()
{
	if (m_RigidBody != nullptr)
	{
		return m_RigidBody->getWorldTransform();
	}
	else
	{
		return m_transform;
	}
}

void Entity::setTransform(Transform transform)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->setWorldTransform(transform);
	}
	else
	{
		m_transform = transform;
	}
}

//Velocity Functions
vector3D Entity::getLinearVelocity() const
{
	if (m_RigidBody != nullptr)
	{
		return m_RigidBody->getLinearVelocity();
	}
	else
	{
		return m_velocity.linearVelocity;
	}
}

void Entity::setLinearVelocity(vector3D velocity)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->setLinearVelocity(velocity);
	}
	else
	{
		m_velocity.linearVelocity = velocity;
	}
}

vector3D Entity::getAngularVelocity() const
{
	if (m_RigidBody != nullptr)
	{
		return m_RigidBody->getAngularVelocity();
	}
	else
	{
		return m_velocity.angularVelocity;
	}
}

void Entity::setAngularVelocity(vector3D velocity)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->setAngularVelocity(velocity);
	}
	else
	{
		m_velocity.angularVelocity = velocity;
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
