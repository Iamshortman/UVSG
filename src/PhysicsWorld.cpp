#include "PhysicsWorld.hpp"
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
	dynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));
}

void PhysicsWorld::update(EntityX &entitySystem, float timeStep)
{
	ComponentHandle<RigidBody> componentRigidBodySearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentRigidBodySearch))
	{
		ComponentHandle<RigidBody> componentRigidBody = entity.component<RigidBody>();
		
		//Do nothing if the entity is not part of this world
		if (componentRigidBody->world != this)
		{
			continue;
		}


		btTransform transform;
		componentRigidBody->motionState->getWorldTransform(transform);

		if (entity.has_component<Transform>())
		{
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			vector3 vec = componentTransform->position;
			transform.setOrigin(toBtVec3(vec));

			quaternion quat = componentTransform->orientation;
			transform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));
		}

		componentRigidBody->motionState->setWorldTransform(transform);
		componentRigidBody->rigidBody->setCenterOfMassTransform(transform);

		if (entity.has_component<Velocity>())
		{
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();
			vector3 linear = componentVelocity->linearVelocity;
			componentRigidBody->rigidBody->setLinearVelocity(toBtVec3(linear));
			vector3 angular = componentVelocity->angularVelocity;
			componentRigidBody->rigidBody->setAngularVelocity(toBtVec3(angular));
			if (linear.x != 0 || linear.y != 0 || linear.z != 0 || angular.x != 0 || angular.y != 0 || angular.z != 0)
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

		//Do nothing if the entity is not part of this world
		if (componentRigidBody->world != this)
		{
			continue;
		}

		btTransform transform;
		componentRigidBody->motionState->getWorldTransform(transform);

		if (entity.has_component<Transform>())
		{
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			btVector3 position = transform.getOrigin();
			componentTransform->position = toGlmVec3(position);

			btQuaternion orientation = transform.getRotation();

			componentTransform->orientation = quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ());
		}

		if (entity.has_component<Velocity>())
		{
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();

			btVector3 linear = componentRigidBody->rigidBody->getLinearVelocity();
			componentVelocity->linearVelocity = toGlmVec3(linear);

			btVector3 angular = componentRigidBody->rigidBody->getAngularVelocity();
			componentVelocity->angularVelocity = toGlmVec3(angular);
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

RayTestResult PhysicsWorld::rayTest(vector3 startPos, vector3 endPos)
{
	btVector3 start = toBtVec3(startPos);
	btVector3 end = toBtVec3(endPos);

	btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
	dynamicsWorld->rayTest(start, end, rayCallback);
	
	RayTestResult result = RayTestResult(rayCallback.hasHit());

	//If it did hit something
	if (rayCallback.hasHit())
	{
		const btRigidBody* rigidBody = btRigidBody::upcast(rayCallback.m_collisionObject);

		result.entityID = rigidBody->getUserIndex();

		result.worldHitPoint = toGlmVec3(rayCallback.m_hitPointWorld);
		result.worldHitNormal = toGlmVec3(rayCallback.m_hitNormalWorld);
	}

	return result;
}

btVector3 PhysicsWorld::toBtVec3(const vector3& vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

vector3 PhysicsWorld::toGlmVec3(const btVector3& vec)
{
	return vector3(vec.getX(), vec.getY(), vec.getZ());
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