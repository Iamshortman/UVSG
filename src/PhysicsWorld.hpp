#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "BulletDynamics\Character\btKinematicCharacterController.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

#include "Components.hpp"
#include "glmInclude.hpp"
#include "Util.hpp"
#include "entityxInclude.hpp"

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
	btDiscreteDynamicsWorld* dynamicsWorld;

protected:

private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	
};

#endif //PHYSICSWORLD_HPP
