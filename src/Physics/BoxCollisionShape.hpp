#ifndef BOXCOLLISIONSHAPE_HPP
#define BOXCOLLISIONSHAPE_HPP

#include "CollisionShape.hpp"

class BoxCollisionShape : public CollisionShape
{
public:
	vector3D m_halfExtends;

	BoxCollisionShape(vector3D halfExtends)
	{
		m_halfExtends = halfExtends;
	};

	virtual btCollisionShape* getCollisionShape()
	{
		return new btBoxShape(toBtVec3(m_halfExtends));
	};
};

#endif //BOXCOLLISIONSHAPE_HPP