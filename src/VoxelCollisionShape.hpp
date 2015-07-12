#ifndef VOXELCOLLISIONSHAPE_HPP
#define VOXELCOLLISIONSHAPE_HPP

#include <btBulletCollisionCommon.h>
#include <glmInclude.hpp>

#include <map>

struct ChildShapeData
{
	btTransform offsetFromBlockPos;
	btCollisionShape* childShape;
};


class VoxelCollisionShape : public btCollisionShape
{
	const static unsigned int chunkSize = 16;
	btVector3 localAabbMin;
	btVector3 localAabbMax;
	btVector3 m_localScaling;
	btScalar m_collisionMargin;


	void addChild(int x, int y, int z, btTransform& transform, btCollisionShape* shape);
	btCollisionShape* getChildShape(int x, int y, int z);
	btTransform getChildTransform(int x, int y, int z);

	virtual	void getAabb(const btTransform& t, btVector3& aabbMin, btVector3& aabbMax) const;
	virtual void recalculateLocalAabb();

	virtual void setLocalScaling(const btVector3& scaling);

	virtual const btVector3& getLocalScaling() const
	{
		return m_localScaling;
	}

	virtual void calculateLocalInertia(btScalar mass, btVector3& inertia) const;

	virtual void setMargin(btScalar margin)
	{
		m_collisionMargin = margin;
	}
	virtual btScalar getMargin() const
	{
		return m_collisionMargin;
	}
	virtual const char*	getName()const
	{
		return "Voxel";
	}

private:
	std::map<vector3I, ChildShapeData> blocks;
};



#endif //VOXELCOLLISIONSHAPE_HPP