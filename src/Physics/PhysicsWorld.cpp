#include "PhysicsWorld.hpp"
#include "RigidBody.hpp"
#include "World/World.hpp"

#include <iostream>

bool collisonCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
{
	Entity* entity1 = (Entity*)colObj0Wrap->getCollisionObject()->getUserPointer();

	if (entity1 == nullptr)
	{
		return true;
	}

	return true;
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
	dynamicsWorld->setGravity(btVector3(0.0, 0.0, 0.0));

	gContactAddedCallback = collisonCallback;
}

void PhysicsWorld::update(double timeStep)
{
	//Run Physics Simulation
	dynamicsWorld->stepSimulation(timeStep, 8, 1.0/120.0);
}

void PhysicsWorld::addRigidBody(btRigidBody* body)
{
	dynamicsWorld->addRigidBody(body);
}

void PhysicsWorld::removeRigidBody(btRigidBody* body)
{
	dynamicsWorld->removeRigidBody(body);
}

SingleRayTestResult PhysicsWorld::singleRayTest(vector3D startPos, vector3D endPos)
{
	btVector3 start = toBtVec3(startPos);
	btVector3 end = toBtVec3(endPos);

	btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);

	// Perform raycast
	dynamicsWorld->rayTest(start, end, rayCallback);

	SingleRayTestResult result;

	if (rayCallback.hasHit())
	{
		result.hasHit = true;
		const btRigidBody* hitBody = btRigidBody::upcast(rayCallback.m_collisionObject);
		result.hitBody = hitBody;
		result.hitPosition = toGlmVec3(rayCallback.m_hitPointWorld);
		result.hitNormal = toGlmVec3(rayCallback.m_hitNormalWorld);
		result.hitEntity = (Entity*)hitBody->getUserPointer();
		result.index = rayCallback.m_partId;

		const btCollisionShape* shape = hitBody->getCollisionShape();
		if (shape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
		{
			btCompoundShape* compoundShape = (btCompoundShape*)hitBody->getCollisionShape();
			result.userValue = compoundShape->getChildShape(result.index)->getUserIndex();
		}
	}

	return result;
}

SingleRayTestResult PhysicsWorld::singleRayTestNotMe(vector3D startPos, vector3D endPos, btCollisionObject* me)
{
	btVector3 start = toBtVec3(startPos);
	btVector3 end = toBtVec3(endPos);

	btCollisionWorld::AllHitsRayResultCallback rayCallback(start, end);

	// Perform raycast
	dynamicsWorld->rayTest(start, end, rayCallback);

	SingleRayTestResult result;

	if (rayCallback.hasHit())
	{
		int closestHitIndex = -1;

		for (int i = 0; i < rayCallback.m_collisionObjects.size(); i++)
		{
			if (rayCallback.m_collisionObjects[i] != me)
			{
				if (closestHitIndex != -1)
				{
					btVector3 distance1 = rayCallback.m_hitPointWorld[i] - start;
					btVector3 distance2 = rayCallback.m_hitPointWorld[closestHitIndex] - start;
					if (distance1.length() < distance2.length())
					{
						closestHitIndex = i;
					}
				}
				else
				{
					closestHitIndex = i;
				}
			}

		}

		if (closestHitIndex != -1)
		{
			result.hasHit = true;
			const btRigidBody* hitBody = btRigidBody::upcast(rayCallback.m_collisionObjects[closestHitIndex]);
			result.hitBody = hitBody;
			result.hitPosition = toGlmVec3(rayCallback.m_hitPointWorld[closestHitIndex]);
			result.hitNormal = toGlmVec3(rayCallback.m_hitNormalWorld[closestHitIndex]);
			result.hitEntity = (Entity*)hitBody->getUserPointer();
		}

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