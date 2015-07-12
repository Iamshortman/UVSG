#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include <entityx\entityx.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "Components.hpp"
#include "glmInclude.hpp"


class PhysicsWorld
{

public:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void update(entityx::EntityX &ex, float deltaTime);
	void addRigidBody(btRigidBody* body);

protected:

private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

};

#endif //PHYSICSWORLD_HPP