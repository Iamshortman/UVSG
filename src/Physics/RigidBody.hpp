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

	void addToPhysicsWorld(PhysicsWorld* physicsWorld, Entity* entity, Transform worldTransform);

	void setCollisionShape(bool deleteOldShape, btCollisionShape* newShape);

	Transform getWorldTransform();
	void setWorldTransform(Transform transform);

	Transform getCOMTransform();
	void setCOMTransform(Transform transform);

	vector3D getLinearVelocity() const;
	void setLinearVelocity(vector3D velocity);

	vector3D getAngularVelocity() const;
	void setAngularVelocity(vector3D velocity);

	PhysicsWorld* getPhysicsWorld() const;

private:
	btRigidBody* rigidBody;

	PhysicsWorld* world = nullptr;
};

#endif //RIGIDBODY_HPP