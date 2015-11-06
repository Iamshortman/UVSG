#include "PhysicsWorld.hpp"
#include "RigidBody.hpp"
#include <iostream>

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
	dynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));
}

void PhysicsWorld::update(EntityX &entitySystem, float timeStep)
{
	ComponentHandle<RigidBody> componentRigidBodySearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentRigidBodySearch))
	{
		ComponentHandle<RigidBody> componentRigidBody = entity.component<RigidBody>();

		if (entity.has_component<Transform>())
		{
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			componentRigidBody->setWorldTranform( *componentTransform.get() );
		}

		if (entity.has_component<Velocity>())
		{
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();
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

	for (Entity entity : entitySystem.entities.entities_with_components(componentRigidBodySearch))
	{
		ComponentHandle<RigidBody> componentRigidBody = entity.component<RigidBody>();

		if (entity.has_component<Transform>())
		{
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			componentTransform->setTransform(componentRigidBody->getWorldTransform());
		}

		if (entity.has_component<Velocity>())
		{
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();

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

SingleRayTestResults PhysicsWorld::singleRayTest(vector3 startPos, vector3 endPos)
{
	btVector3 start = toBtVec3(startPos);
	btVector3 end = toBtVec3(endPos);

	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);

	// Perform raycast
	dynamicsWorld->rayTest(start, end, RayCallback);

	SingleRayTestResults result;

	if (RayCallback.hasHit())
	{
		result.hasHit = true;
		const btRigidBody* hitBody = btRigidBody::upcast(RayCallback.m_collisionObject);
		result.hitBody = hitBody;
		result.hitPosition = toGlmVec3(RayCallback.m_hitPointWorld);
		result.hitNormal = toGlmVec3(RayCallback.m_hitNormalWorld);



	}

	return result;
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