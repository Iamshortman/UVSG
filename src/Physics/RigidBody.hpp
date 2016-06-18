#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <btBulletDynamicsCommon.h>

#include "GLM_Include.hpp"
#include "Components/Velocity.hpp"
#include "Components/Transform.hpp"

class PhysicsWorld;
class Entity;

class RigidBody
{
public:
	RigidBody(btCollisionShape* shape, btScalar mass);
	RigidBody(btCollisionShape* shape, btScalar mass, const btVector3& inertia);

	virtual ~RigidBody();

	void addToPhysicsWorld(PhysicsWorld* physicsWorld, Entity* entity, Transform worldTransform);
	void removeFromPhysicsWorld();
	bool isInPhysicsWorld();

	void Activate(bool activate);

	void setCollisionShape(bool deleteOldShape, btCollisionShape* newShape);

	Transform getWorldTransform();
	void setWorldTransform(Transform transform);

	Transform getCOMTransform();
	void setCOMTransform(Transform transform);

	vector3D getLinearVelocity() const;
	void setLinearVelocity(vector3D velocity);

	vector3D getAngularVelocity() const;
	void setAngularVelocity(vector3D velocity);

	void applyCentralForce(vector3D force);
	void applyCentralImpulse(vector3D impulse);

	void applyTorque(vector3D torque);
	void applyTorqueImpulse(vector3D torque);

	void setDampening(double linear, double angular);

	PhysicsWorld* getPhysicsWorld() const;

private:
	btRigidBody* rigidBody;

	PhysicsWorld* world = nullptr;
};

#endif //RIGIDBODY_HPP