#include "World.hpp"
#include <iostream>

World::World()
{
    worldPhysics = new PhysicsWorld();
    gameObjects = std::vector<GameObject*>();
}

unsigned int World::createCube(btVector3 pos, btVector3 size)
{
    unsigned int id = gameObjects.size();
    GameObject* cube = new GameObject(id);

    btCollisionShape* boxShape = new btBoxShape(size);
    btScalar mass = 1;
    btVector3 boxInertia;
    boxShape->calculateLocalInertia(mass, boxInertia);
    btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
    btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, boxMotionState, boxShape, boxInertia);
    btRigidBody* boxRigidBody = new btRigidBody(boxRigidBodyCI);
    cube->addRigidBody(boxRigidBody);
    worldPhysics->addRigidBody(boxRigidBody);
    gameObjects.push_back(cube);

    return id;
}

void World::deleteGameObject(unsigned int object_id)
{
    GameObject* object = gameObjects[object_id];
    if(object != 0)
    {
        worldPhysics->removeRigidBody(object->body);
        //gameObjects.erase(gameObjects.begin() + object_id);
        delete object;
        //To Preserve gameObjects ids, just make the pointer 0
        gameObjects[object_id] = 0;
    }
}

GameObject* World::getGameObject(unsigned int object_id)
{
    if(object_id >= gameObjects.size())
    {
        return 0;
    }

    return gameObjects[object_id];
}

void World::update(unsigned long delta)
{
    float timeStep = delta;
    timeStep /= 1000.0f;
    worldPhysics->updateWorld(timeStep);
}

GameObject* World::rayTrace(btVector3 start, btVector3 end)
{
    ClosestRayResultCallback result = worldPhysics->rayTest(start, end);
    if(result.hasHit())
    {
        const btRigidBody* rigidBody = btRigidBody::upcast(result.m_collisionObject);
        GameObject* gameObject = static_cast<GameObject*>(rigidBody->getUserPointer());
        return gameObject;
    }

    return 0;
}

int World::getGameObjectCount()
{
    return gameObjects.size();
}

World::~World()
{
    delete worldPhysics;
}
