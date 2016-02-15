#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <btBulletDynamicsCommon.h>
#include "glmInclude.hpp"
#include "Components.hpp"

class PhysicsWorld;

class RigidBody
{
public:
	RigidBody(PhysicsWorld* physicsWorld, btCollisionShape* shape, btScalar mass);
	RigidBody(PhysicsWorld* physicsWorld, btCollisionShape* shape, btScalar mass, const btVector3& inertia);

	virtual ~RigidBody();

	void setCollisionShape(bool deleteOldShape, btCollisionShape* newShape);

	Transform getWorldTransform();
	void setWorldTranform(Transform transform);


	btRigidBody* rigidBody;

	void setObjectKinematic(bool kinematic){ isKinematic = kinematic; };
	bool isObjectKinematic(){ return isKinematic; };
	PhysicsWorld* getPhysicsWorld() const;

private:
	PhysicsWorld* world;
	bool isKinematic = false;
};

#endif //RIGIDBODY_HPP