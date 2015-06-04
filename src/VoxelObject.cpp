#include "VoxelObject.hpp"
#include "World.hpp"
#include "PhysicsWorld.hpp"
#include "RigidBodyComponent.hpp"

#include <iostream>

VoxelObject::VoxelObject(unsigned int id, float size):
GameObject(id),
cubeSize(size)
{

}

void VoxelObject::update()
{
    GameObject::update();
    btTransform temp;
    body->getMotionState()->getWorldTransform(temp);
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
