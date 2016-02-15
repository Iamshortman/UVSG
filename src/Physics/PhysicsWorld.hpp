#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "EntityxInclude.hpp"
#include "Components.hpp"
#include "glmInclude.hpp"
#include "Util.hpp"

struct SingleRayTestResults
{
	bool hasHit = false;
	const btRigidBody* hitBody;
	vector3D hitPosition;
	vector3D hitNormal;
};


class PhysicsWorld
{

public:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void update(EntityX &entitySystem, double deltaTime);
	void addRigidBody(btRigidBody* body);
	void removeRigidBody(btRigidBody* body);

	SingleRayTestResults singleRayTest(vector3D startPos, vector3D endPos);
	btDiscreteDynamicsWorld* dynamicsWorld;

protected:

private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

};

#endif //PHYSICSWORLD_HPP
