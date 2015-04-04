#include "World.hpp"
#include "GameObject.hpp"
#include <iostream>

#include "RigidBodyComponent.hpp"

World::World()
{
    worldPhysics = new PhysicsWorld();
    gameObjects = std::vector<GameObject*>();
}

unsigned int World::createCube(btVector3 pos, btVector3 size)
{
    unsigned int id = gameObjects.size();
    GameObject* cube = new GameObject(id);
    cube->setWorldPtr(this);

    btCollisionShape* boxShape = new btBoxShape(size);
    btScalar mass = 1;
    cube->transform = btTransform(btQuaternion(0, 0, 0, 1), pos);
    cube->addComponent(new RigidBodyComponent(cube, cube->transform, boxShape, mass));
    gameObjects.push_back(cube);

    return id;
}

void World::deleteGameObject(unsigned int object_id)
{
    GameObject* object = gameObjects[object_id];
    if(object != 0)
    {
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

    for(unsigned int i = 0; i < gameObjects.size(); i++)
    {
        if(gameObjects[i] != 0)
        {
            gameObjects[i]->update();
        }
    }
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
