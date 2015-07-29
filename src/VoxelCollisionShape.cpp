#include "VoxelCollisionShape.hpp"
/*
VoxelCollisionShape::VoxelCollisionShape()
	:
	m_localAabbMin(btScalar(BT_LARGE_FLOAT), btScalar(BT_LARGE_FLOAT), btScalar(BT_LARGE_FLOAT)),
	m_localAabbMax(btScalar(-BT_LARGE_FLOAT), btScalar(-BT_LARGE_FLOAT), btScalar(-BT_LARGE_FLOAT)),
	m_updateRevision(1),
	m_collisionMargin(btScalar(0.)),
	m_localScaling(btScalar(1.), btScalar(1.), btScalar(1.)),
	blockSize(1.0f)
{
	//m_shapeType = COMPOUND_SHAPE_PROXYTYPE;

}



void VoxelCollisionShape::addChild(int x, int y, int z)
{
	VoxelShapeData data;
	data.blockPos = vector3I(x, y, z);
	data.shape = collisionShapeBox;

	blocks[vector3I(x, y, z)] = data;

	recalculateLocalAabb();
}

void VoxelCollisionShape::recalculateLocalAabb()
{
	// Recalculate the local aabb
	// Brute force, it iterates over all the shapes left.
	m_localAabbMin = btVector3(btScalar(BT_LARGE_FLOAT), btScalar(BT_LARGE_FLOAT), btScalar(BT_LARGE_FLOAT));
	m_localAabbMax = btVector3(btScalar(-BT_LARGE_FLOAT), btScalar(-BT_LARGE_FLOAT), btScalar(-BT_LARGE_FLOAT));

	typedef std::map<vector3I, VoxelShapeData, CompareVectors>::iterator it_type;

	for (it_type iterator = blocks.begin(); iterator != blocks.end(); iterator++)
	{
		btVector3 localAabbMin, localAabbMax;
		if (iterator->second.shape == collisionShapeBox)
		{
			vector3I blockPosVec = iterator->first;
			btVector3 offset = btVector3(blockPosVec.x, blockPosVec.y, blockPosVec.z);
			offset *= blockSize;
			btScalar halfBlockSize = blockSize / 2.0f;
			btBoxShape box = btBoxShape(btVector3(halfBlockSize, halfBlockSize, halfBlockSize));
			btTransform transform = btTransform(btQuaternion(0, 0, 0, 1), offset);
			box.getAabb(transform, localAabbMin, localAabbMax);

			localAabbMin += offset;
			localAabbMax += offset;

			//extend the local aabbMin/aabbMax
			for (int i = 0; i<3; i++)
			{
				if (m_localAabbMin[i] > localAabbMin[i])
					m_localAabbMin[i] = localAabbMin[i];
				if (m_localAabbMax[i] < localAabbMax[i])
					m_localAabbMax[i] = localAabbMax[i];
			}
		}
	}

	int x = 2 + 5;
}

///getAabb's default implementation is brute force, expected derived classes to implement a fast dedicated version
void VoxelCollisionShape::getAabb(const btTransform& trans, btVector3& aabbMin, btVector3& aabbMax) const
{
	btVector3 localHalfExtents = btScalar(0.5)*(m_localAabbMax - m_localAabbMin);
	btVector3 localCenter = btScalar(0.5)*(m_localAabbMax + m_localAabbMin);

	//avoid an illegal AABB when there are no children
	if (!blocks.size())
	{
		localHalfExtents.setValue(0, 0, 0);
		localCenter.setValue(0, 0, 0);
	}
	localHalfExtents += btVector3(getMargin(), getMargin(), getMargin());


	btMatrix3x3 abs_b = trans.getBasis().absolute();

	btVector3 center = trans(localCenter);

	btVector3 extent = localHalfExtents.dot3(abs_b[0], abs_b[1], abs_b[2]);
	aabbMin = center - extent;
	aabbMax = center + extent;

}

void VoxelCollisionShape::calculateLocalInertia(btScalar mass, btVector3& inertia) const
{
	//approximation: take the inertia from the aabb for now
	btTransform ident;
	ident.setIdentity();
	btVector3 aabbMin, aabbMax;
	getAabb(ident, aabbMin, aabbMax);

	btVector3 halfExtents = (aabbMax - aabbMin)*btScalar(0.5);

	btScalar lx = btScalar(2.)*(halfExtents.x());
	btScalar ly = btScalar(2.)*(halfExtents.y());
	btScalar lz = btScalar(2.)*(halfExtents.z());

	inertia[0] = mass / (btScalar(12.0)) * (ly*ly + lz*lz);
	inertia[1] = mass / (btScalar(12.0)) * (lx*lx + lz*lz);
	inertia[2] = mass / (btScalar(12.0)) * (lx*lx + ly*ly);

	inertia.setY(1.0f);
}

void VoxelCollisionShape::setLocalScaling(const btVector3& scaling)
{

}
*/