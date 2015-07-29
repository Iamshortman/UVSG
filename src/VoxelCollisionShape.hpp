#ifndef VOXELCOLLISIONSHAPE_HPP
#define VOXELCOLLISIONSHAPE_HPP

#include <btBulletCollisionCommon.h>
#include <map>

#include "vector3I.hpp"

enum CollisionShape
{
	collisionShapeBox = 1,
};


//A way to compair two vector3I without polluting the original class;
struct CompareVectors
{
	bool operator()(const vector3I& a, const vector3I& b)
	{
		// z trumps, then y, then x
		if (a.z < b.z)
		{
			return true;
		}
		else if (a.z == b.z)
		{
			if (a.y < b.y)
			{
				// a.z == b.z and y < b.y
				return true;
			}
			else if (a.y == b.y)
			{
				if (a.x < b.x)
				{
					return true;
				}
				else if (a.x == b.x)
				{
					// completely equal
					return false;
				}
				else
				{
					return false;
				}
			}
			else
			{
				// z==b.z and y >= b.y
				return false;
			}
		}
		else
		{
			// z >= b.z
			return false;
		}
	}
};

struct VoxelShapeData
{
	vector3I blockPos;
	CollisionShape shape;
	btVector3 size;
	btVector3 offset;
	btQuaternion rotation;
};


class VoxelCollisionShape : public btCompoundShape
{
public:

	const float blockSize;

	virtual const char*	getName()const
	{
		return "Voxel";
	};

	int getNumChildShapes() const
	{
		return blocks.size();
	};

	virtual btCollisionShape* getChildShape(int index);
	virtual const btCollisionShape* getChildShape(int index) const;
	virtual btTransform& getChildTransform(int index);
	virtual const btTransform&	getChildTransform(int index) const;
	virtual void recalculateLocalAabb();
	


private:
	std::map<vector3I, VoxelShapeData, CompareVectors> blocks;
};



#endif //VOXELCOLLISIONSHAPE_HPP