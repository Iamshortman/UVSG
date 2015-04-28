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

unsigned int World::createPlayer(btVector3& pos)
{
    unsigned int id = gameObjects.size();
    PlayerObject* player = new PlayerObject(id);
    player->setWorldPtr(this);

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

unsigned int World::createCube(btVector3& pos, btVector3& size)
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

unsigned int World::createGameObject(btVector3& pos)
{
    unsigned int id = gameObjects.size();
    GameObject* object = new GameObject(id);
    object->setWorldPtr(this);
    gameObjects.push_back(object);

    return id;
}

unsigned int World::createVoxelObject(btVector3& pos)
{
    unsigned int id = gameObjects.size();
    VoxelObject* voxel = new VoxelObject(id);
    voxel->setWorldPtr(this);
    voxel->transform = btTransform(btQuaternion(0, 0, 0, 1), pos);

    unsigned int chunkSize = voxel->chunkSize;

    btScalar mass = 0;

    btCompoundShape* voxels = new btCompoundShape();
    btCollisionShape* boxShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));

    for(unsigned int x = 0; x < chunkSize; x++)
    {
            for(unsigned  int y = 0; y < chunkSize; y++)
            {
                for(unsigned int z = 0; z < chunkSize; z++)
                {
                    if(voxel->chunk[x][y][z] == 1)
                    {
                        voxels->addChildShape(btTransform(btQuaternion(0.0f, 1.0f, 0.0f, 1.0f), btVector3(x, y, z)), boxShape);
                        mass += 1.0f;
                    }
                }
            }
    }
    btVector3 Inertia;
    voxels->calculateLocalInertia(mass, Inertia);
    btDefaultMotionState* MotionState = new btDefaultMotionState(voxel->transform);
    btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, MotionState, voxels, Inertia);
    voxel->body = new btRigidBody(boxRigidBodyCI);
    voxel->body->setUserPointer(voxel);
    worldPhysics->addRigidBody(voxel->body);

    btTransform offsetTransform = btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(4.0f, 4.0f, 4.0f));
    voxel->body->setCenterOfMassTransform(offsetTransform);

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

GameObject* World::rayTrace(btVector3& start, btVector3& end)
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
