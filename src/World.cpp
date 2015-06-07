#include "World.hpp"
#include "GameObject.hpp"
#include <iostream>

#include "RigidBodyComponent.hpp"
#include "MeshComponent.hpp"
#include "Mesh.hpp"

#include "VoxelObject.hpp"
#include "PlayerObject.hpp"

World::World()
{
    worldPhysics = new PhysicsWorld();
    gameObjects = std::vector<GameObject*>();
}

unsigned int World::createPlayer(const  btVector3& pos)
{
    unsigned int id = gameObjects.size();
    PlayerObject* player = new PlayerObject(this, id);

    btVector3 inertia(0.0f, 0.0f, 0.0f);//No inertia since its a player
    btCollisionShape* capsule = new btCapsuleShape(0.4f, 2.0f);
    btScalar mass = 1.0F;
    capsule->calculateLocalInertia(mass, inertia);
    btTransform transform = btTransform(btQuaternion(0, 0, 0, 1), pos);
    btDefaultMotionState* MotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, MotionState, capsule, inertia);
    rigidBodyCI.m_friction = 1.0f;
	rigidBodyCI.m_restitution = 0.0f;
	rigidBodyCI.m_linearDamping = 0.0f;

    player->body = new btRigidBody(rigidBodyCI);
    player->body->setUserPointer(player);
    // Keep upright
	player->body->setAngularFactor(0.0f);

	// No sleeping (or else setLinearVelocity won't work)
	player->body->setActivationState(DISABLE_DEACTIVATION);

    worldPhysics->addRigidBody(player->body);
    gameObjects.push_back(player);

    player->m_pGhostObject = new btPairCachingGhostObject();
    player->m_pGhostObject->setCollisionShape(player->body->getCollisionShape());
    player->m_pGhostObject->setUserPointer(player);
	player->m_pGhostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

    worldPhysics->addCollisionObject(player->m_pGhostObject, btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

    return id;
}

unsigned int World::createCube(const btVector3& pos, const btVector3& size)
{
    unsigned int id = gameObjects.size();
    GameObject* cube = new GameObject(this, id);

    btCollisionShape* boxShape = new btBoxShape(size);
    btScalar mass = 1;
    cube->transform = btTransform(btQuaternion(0, 0, 0, 1), pos);
    cube->addComponent(new RigidBodyComponent(cube, cube->transform, boxShape, mass));
    gameObjects.push_back(cube);

    return id;
}

unsigned int World::createGameObject(const btVector3& pos)
{
    unsigned int id = gameObjects.size();
    GameObject* object = new GameObject(this, id);
    gameObjects.push_back(object);

    return id;
}

unsigned int World::createVoxelObject(const btVector3& pos)
{
    unsigned int id = gameObjects.size();
    VoxelObject* voxel = new VoxelObject(this, id, 4.0f);
    voxel->transform = btTransform(btQuaternion(0, 0, 0, 1), pos);
	gameObjects.push_back(voxel);

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

GameObject* World::rayTrace(const btVector3& start, const btVector3& end)
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
