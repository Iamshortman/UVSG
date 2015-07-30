#include "PhysicsWorld.hpp"
#include <iostream>

void printQuat(const quaternion quat)
{
	std::cout << "{ " << quat.x << ", " << quat.y << ", " << quat.z << ", " << quat.w << "} \n";
}

void printVec(const vector3 quat)
{
	std::cout << "{ " << quat.x << ", " << quat.y << ", " << quat.z << "} \n";
}

PhysicsWorld::PhysicsWorld()
{
	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));
}

void PhysicsWorld::update(entityx::EntityX &entitySystem, float timeStep)
{
	entityx::ComponentHandle<RigidBody> componentRigidBodySearch;
	for (entityx::Entity entity : entitySystem.entities.entities_with_components(componentRigidBodySearch))
	{
		entityx::ComponentHandle<RigidBody> componentRigidBody = entity.component<RigidBody>();

		btTransform transform;
		componentRigidBody->motionState->getWorldTransform(transform);

		if (entity.has_component<Transform>())
		{
			entityx::ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			vector3 vec = componentTransform->position;
			transform.setOrigin(btVector3(vec.x, vec.y, vec.z));

			quaternion quat = componentTransform->orientation;
			transform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));
		}

		componentRigidBody->motionState->setWorldTransform(transform);
		componentRigidBody->rigidBody->setCenterOfMassTransform(transform);

		if (entity.has_component<Velocity>())
		{
			entityx::ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();
			vector3 linear = componentVelocity->linearVelocity;
			componentRigidBody->rigidBody->setLinearVelocity(btVector3(linear.x, linear.y, linear.z));
			vector3 angular = componentVelocity->angularVelocity;
			componentRigidBody->rigidBody->setAngularVelocity(btVector3(angular.x, angular.y, angular.z));
			if (linear.x || linear.y || linear.z || angular.x || angular.y || angular.z)
			{
				componentRigidBody->rigidBody->activate(true);
			}
		}
	}

	//Run Physics Simulation
	dynamicsWorld->stepSimulation(timeStep, 0, (1.0f / 120.0f));

	for (entityx::Entity entity : entitySystem.entities.entities_with_components(componentRigidBodySearch))
	{
		entityx::ComponentHandle<RigidBody> componentRigidBody = entity.component<RigidBody>();

		btTransform transform;
		componentRigidBody->motionState->getWorldTransform(transform);

		if (entity.has_component<Transform>())
		{
			entityx::ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			btVector3 position = transform.getOrigin();
			componentTransform->position = vector3(position.getX(), position.getY(), position.getZ());

			btQuaternion orientation = transform.getRotation();

			componentTransform->orientation = quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ());
		}

		if (entity.has_component<Velocity>())
		{
			entityx::ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();

			btVector3 linear = componentRigidBody->rigidBody->getLinearVelocity();
			componentVelocity->linearVelocity = vector3(linear.getX(), linear.getY(), linear.getZ());

			btVector3 angular = componentRigidBody->rigidBody->getAngularVelocity();
			componentVelocity->angularVelocity = vector3(angular.getX(), angular.getY(), angular.getZ());
		}
	}

}

void PhysicsWorld::addRigidBody(btRigidBody* body)
{
	dynamicsWorld->addRigidBody(body);
}

void PhysicsWorld::removeRigidBody(btRigidBody* body)
{
	dynamicsWorld->removeRigidBody(body);
}

PhysicsWorld::~PhysicsWorld()
{
	// Clean up behind ourselves like good little programmers
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
}