#include "VoxelCollisionShape.hpp"

void VoxelCollisionShape::addChild(int x, int y, int z, btTransform& transform, btCollisionShape* shape)
{
	ChildShapeData data = {transform, shape};

	blocks[vector3I(x, y, z)] = data;
}

btCollisionShape* VoxelCollisionShape::getChildShape(int x, int y, int z)
{
	return blocks[vector3I(x, y, z)].childShape;
}

btTransform VoxelCollisionShape::getChildTransform(int x, int y, int z)
{
	return blocks[vector3I(x, y, z)].offsetFromBlockPos;
}