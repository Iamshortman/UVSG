#ifndef MESHCOLLISIONSHAPE_HPP
#define MESHCOLLISIONSHAPE_HPP

#include "CollisionShape.hpp"
#include "Rendering/ObjLoader.hpp"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

class MeshCollisionShape : public CollisionShape
{
public:
	btConvexHullShape* hull;

	MeshCollisionShape(std::string filePath, std::string fileName)
	{
		hull = new btConvexHullShape();
		loadConvexHull(filePath, fileName, hull);
	};

	~MeshCollisionShape()
	{
		delete hull;
	};

	virtual btCollisionShape* getCollisionShape()
	{
		btShapeHull* tempHull = new btShapeHull(hull);
		btScalar margin = hull->getMargin();
		tempHull->buildHull(margin);
		btConvexHullShape* simplifiedConvexShape = new btConvexHullShape(*tempHull->getVertexPointer(), tempHull->numVertices());

		delete tempHull;

		return simplifiedConvexShape;
	};
};

#endif //MESHCOLLISIONSHAPE_HPP
