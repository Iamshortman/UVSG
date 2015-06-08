#include "PhysicsWorld.hpp"

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
    //dynamicsWorld->setGravity(btVector3(0.0F, -9.8F, 0.0F));
	dynamicsWorld->setGravity(btVector3(0.0F, 0.0F, 0.0F));

}

void PhysicsWorld::updateWorld(float timeStep)
{
    dynamicsWorld->stepSimulation(timeStep, 4);
}

void PhysicsWorld::addRigidBody(btRigidBody* body)
{
    dynamicsWorld->addRigidBody(body);
}

void PhysicsWorld::addCollisionObject(btCollisionObject *collisionObject, short int collisionFilterGroup, short int collisionFilterMask)
{
    dynamicsWorld->addCollisionObject(collisionObject, collisionFilterGroup, collisionFilterMask);
}

void PhysicsWorld::removeRigidBody(btRigidBody* body)
{
    dynamicsWorld->removeRigidBody(body);
}

ClosestRayResultCallback PhysicsWorld::rayTest(const btVector3& start, const btVector3& end)
{
    ClosestRayResultCallback RayCallback(start, end);
    dynamicsWorld->rayTest(start, end, RayCallback);
    return RayCallback;
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
