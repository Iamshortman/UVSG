#ifndef COLLISIONSHAPE_HPP
#define COLLISIONSHAPE_HPP

#include "Util.hpp"
#include "GLM_Include.hpp"
#include <btBulletCollisionCommon.h>

class CollisionShape
{
public:
	virtual btCollisionShape* getCollisionShape() = 0;
};

#endif //COLLISIONSHAPE_HPP