#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include <entityxInclude.hpp>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "Components.hpp"
#include "glmInclude.hpp"
#include "Util.hpp"

struct RayTestResult
{
	RayTestResult(bool hasHitEntity)
	{
		hasHit = hasHitEntity;
	};

	bool hasHit;
	vector3 worldHitPoint;
	vector3 worldHitNormal;
	entityxId entityID;
};

class PhysicsWorld
{

public:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void update(EntityX &ex, float deltaTime);
	void addRigidBody(btRigidBody* body);
	void removeRigidBody(btRigidBody* body);

	RayTestResult rayTest(vector3 startPos, vector3 endPos);


protected:

private:

	btVector3 toBtVec3(const vector3& vec);
	vector3 toGlmVec3(const btVector3& vec);

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

};

#endif //PHYSICSWORLD_HPP