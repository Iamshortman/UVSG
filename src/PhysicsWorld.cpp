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
    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);

    btCollisionShape* boxShape = new btBoxShape(btVector3(1.0F, 1.0F, 1.0F));
    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    boxShape->calculateLocalInertia(mass, fallInertia);
    btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    boxRigidBodyCI(mass, boxMotionState, boxShape, fallInertia);
    boxRigidBody = new btRigidBody(boxRigidBodyCI);
    dynamicsWorld->addRigidBody(boxRigidBody);

}

void PhysicsWorld::updateWorld()
{
    dynamicsWorld->stepSimulation(1 / 60.f, 10);
}



PhysicsWorld::~PhysicsWorld()
{
    dynamicsWorld->removeRigidBody(groundRigidBody);
    delete groundRigidBody->getMotionState();
    delete groundRigidBody->getCollisionShape();
    delete groundRigidBody;

    dynamicsWorld->removeRigidBody(boxRigidBody);
    delete boxRigidBody->getMotionState();
    delete boxRigidBody->getCollisionShape();
    delete boxRigidBody;

    // Clean up behind ourselves like good little programmers
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}
