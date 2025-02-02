#include "World/Entity.hpp"
#include "World/World.hpp"

//Resolve the prototype class
#include "Renderer/Renderer.hpp"

Entity::Entity(EntityId id)
	:m_entityId(id)
{

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

void Entity::kill()
{
	this->m_alive = false;
}

void Entity::update(double deltaTime)
{
	//Not a huge fan of the auto keyword
	//But what are you going to do
	for (auto iterator : m_components)
	{
		iterator.second->update(deltaTime);
	}

	if (ridingEntity == nullptr)
	{
		//Move Entity based on velocity even if there is no RigidBody.
		if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
		{
			//If there is a RigidBody, sync the entities values too the Rigid Bodies values
			vector3D scale = m_transform.getScale();
			m_transform = getTransform();
			m_transform.setScale(scale);
			m_velocity.linearVelocity = getLinearVelocity();
			m_velocity.angularVelocity = getAngularVelocity();

			if (m_velocity.linearVelocity.x || m_velocity.linearVelocity.y || m_velocity.linearVelocity.z
				|| m_velocity.angularVelocity.x || m_velocity.angularVelocity.y || m_velocity.angularVelocity.z)
			{
				m_RigidBody->Activate(true);
			}
			else
			{
				m_RigidBody->Activate(false);
			}
		}
		else
		{
			Transform transform = getTransform();

			//Move Entity Based on Velocity
			transform.setPosition(transform.getPosition() + (getLinearVelocity() * deltaTime));

			//TODO figure out angular Velocity.

			setTransform(transform);
		}
	}

	/*if (riddenByEntity != nullptr)
	{
		Transform transform = this->getTransform();
		vector3D pos = transform.getOrientation() * ridingOffset;
		transform.m_position += pos;

		riddenByEntity->setTransform(transform);
	}*/
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

	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
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
	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
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
	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
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
	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
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
	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
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
	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
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
	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
	{
		m_RigidBody->setAngularVelocity(velocity);
	}
	else
	{
		m_velocity.angularVelocity = velocity;
	}
}

void Entity::applyCentralForce(vector3D force)
{
	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
	{
		m_RigidBody->applyCentralForce(force);
	}
	else
	{

	}
}

void Entity::applyCentralImpulse(vector3D impulse)
{
	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
	{
		m_RigidBody->applyCentralImpulse(impulse);
	}
	else
	{

	}
}

void Entity::applyTorque(vector3D torque)
{
	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
	{
		m_RigidBody->applyTorque(torque);
	}
	else
	{
		
	}
}

void Entity::applyTorqueImpulse(vector3D torque)
{
	if (m_RigidBody != nullptr && m_RigidBody->isInPhysicsWorld())
	{
		m_RigidBody->applyTorqueImpulse(torque);
	}
	else
	{

	}
}

void Entity::setDampening(double linear, double angular)
{
	if (m_RigidBody != nullptr)
	{
		m_RigidBody->setDampening(linear, angular);
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

Transform Entity::getRenderTransform()
{
	Transform worldTransform = m_world->getWorldOffsetMatrix();
	Transform transform = getTransform();

	transform.setOrientation(worldTransform.getOrientation() * transform.getOrientation());
	transform.setPosition(worldTransform.getPosition() + (worldTransform.getOrientation() * transform.getPosition()));

	return transform;
}