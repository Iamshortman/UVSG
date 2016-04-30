#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <btBulletDynamicsCommon.h>
#include "glmInclude.hpp"
#include "Components.hpp"

class PhysicsWorld;
class Entity;

class RigidBody
{
public:
	RigidBody(btCollisionShape* shape, btScalar mass);
	RigidBody(btCollisionShape* shape, btScalar mass, const btVector3& inertia);

	virtual ~RigidBody();

	void addToPhysicsWorld(PhysicsWorld* physicsWorld, Entity* entity);

	void setCollisionShape(bool deleteOldShape, btCollisionShape* newShape);

	Transform getWorldTransform();
	void setWorldTranform(Transform transform);

	Transform getCOMTransform();
	void setCOMTransform(Transform transform);

	btRigidBody* rigidBody;

	void setObjectKinematic(bool kinematic){ isKinematic = kinematic; };
	bool isObjectKinematic(){ return isKinematic; };
	PhysicsWorld* getPhysicsWorld() const;

private:
	PhysicsWorld* world = nullptr;
	bool isKinematic = false;
};

#endif //RIGIDBODY_HPP