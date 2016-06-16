#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "Components/Velocity.hpp"
#include "Components/Transform.hpp"
#include "GLM_Include.hpp"
#include "Util.hpp"
#include "World/Entity.hpp"

struct SingleRayTestResult
{
	Entity* hitEntity = nullptr;
	bool hasHit = false;
	const btRigidBody* hitBody;
	vector3D hitPosition;
	vector3D hitNormal;
	int index = 0;
	int userValue = 0;
};

class PhysicsWorld
{

public:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void update(double deltaTime);
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
