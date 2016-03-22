#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "Components.hpp"
#include "glmInclude.hpp"
#include "Util.hpp"
#include "World/Entity.hpp"

struct SingleRayTestResult
{
	Entity* hitEntity = nullptr;
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
	void update(double deltaTime, vector<Entity*> &entities);
	void addRigidBody(btRigidBody* body);
	void removeRigidBody(btRigidBody* body);

	SingleRayTestResult singleRayTest(vector3D startPos, vector3D endPos);
	SingleRayTestResult singleRayTestNotMe(vector3D startPos, vector3D endPos, btCollisionObject* me);
	btDiscreteDynamicsWorld* dynamicsWorld;

protected:

private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
};

#endif //PHYSICSWORLD_HPP
