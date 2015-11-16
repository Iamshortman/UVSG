#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include <entityxInclude.hpp>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "Components.hpp"
#include "glmInclude.hpp"
#include "Util.hpp"

struct SingleRayTestResults
{
	bool hasHit = false;
	const btRigidBody* hitBody;
	vector3 hitPosition;
	vector3 hitNormal;
};

class PhysicsWorld
{

public:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void update(EntityX &ex, float deltaTime);
	void addRigidBody(btRigidBody* body);
	void removeRigidBody(btRigidBody* body);

	SingleRayTestResults singleRayTest(vector3 startPos, vector3 endPos);

protected:

private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif //PHYSICSWORLD_HPP