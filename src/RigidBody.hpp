#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <btBulletDynamicsCommon.h>
#include "entityxInclude.hpp"
#include "glmInclude.hpp"
#include "Components.hpp"

class PhysicsWorld;

class RigidBody
{
public:
	RigidBody(PhysicsWorld* physicsWorld, Entity entity, btCollisionShape* shape, btScalar mass);
	RigidBody(PhysicsWorld* physicsWorld, btCollisionShape* shape, btScalar mass);
	virtual ~RigidBody();

	void setCollisionShape(bool deleteOldShape, btCollisionShape* newShape);

	Transform getWorldTransform();
	void setWorldTranform(Transform transform);


	btRigidBody* rigidBody;

private:
	PhysicsWorld* world;
};

#endif //RIGIDBODY_HPP