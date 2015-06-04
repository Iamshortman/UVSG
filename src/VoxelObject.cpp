#include "VoxelObject.hpp"
#include "World.hpp"
#include "PhysicsWorld.hpp"
#include "RigidBodyComponent.hpp"

#include <iostream>

VoxelObject::VoxelObject(World* worldPtr, unsigned int id, float size) :
GameObject(worldPtr, id),
cubeSize(size)
{
	initPhysics();
	for (unsigned int x = 0; x < chunkSize; x++)
	{
		for (unsigned int y = 0; y < chunkSize; y++)
		{
			for (unsigned int z = 0; z < chunkSize; z++)
			{
				setBlock(x, y, z, 1);
			}
		}
	}
}

void VoxelObject::initPhysics()
{
	btScalar mass = 1.0f;
	btVector3 Inertia;
	voxels = new btCompoundShape();
	voxels->calculateLocalInertia(mass, Inertia);
	btDefaultMotionState* MotionState = new btDefaultMotionState(this->transform);
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, MotionState, voxels, Inertia);
	rigidBody = new btRigidBody(boxRigidBodyCI);
	rigidBody->setUserPointer(this);
	worldPtr->worldPhysics->addRigidBody(rigidBody);

	collisionChunk[0][0][0] = new btBoxShape(btVector3(1, 1, 1));
	voxels->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)), collisionChunk[0][0][0]);
}

void VoxelObject::update()
{
    GameObject::update();
    btTransform temp;
    rigidBody->getMotionState()->getWorldTransform(temp);
    this->transform = temp;

	if (shouldUpdateChunk)
	{
		updateChunk();
	}
}

void VoxelObject::render()
{
    GameObject::render();
    voxelMesh.draw();
}

void VoxelObject::setUpdateChunk()
{
	shouldUpdateChunk = true;
}

void VoxelObject::setBlock(int x, int y, int z, BlockID id)
{
	if (x >= 0 && x < chunkSize)
	{
		if (y >= 0 && y < chunkSize)
		{
			if (z >= 0 && z < chunkSize)
			{
				chunk[x][y][z] = id;
				shouldUpdateChunk = true;
			}
		}
	}
}

BlockID VoxelObject::getBlock(int x, int y, int z)
{
	if (x >= 0 && x < chunkSize)
	{
		if (y >= 0 && y < chunkSize)
		{
			if (z >= 0 && z < chunkSize)
			{
				return chunk[x][y][z];
			}
		}
	}
	return 0;
}

void VoxelObject::updateChunk()
{
	
}

VoxelObject::~VoxelObject()
{

}
