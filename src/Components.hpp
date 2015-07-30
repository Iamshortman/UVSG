#ifndef COMPONETS_HPP
#define COMPONETS_HPP

#include "Mesh.hpp"
#include <btBulletDynamicsCommon.h>
#include <entityx\entityx.h>

class PhysicsWorld;

class RigidBody
{
public:
	RigidBody(PhysicsWorld* physicsWorld, entityx::Entity entity, btCollisionShape* shape, btScalar mass);
	virtual ~RigidBody();

	btRigidBody* rigidBody;
	btCollisionShape* collisionShape;
	btMotionState* motionState;

private:
	PhysicsWorld* world;
};

class Transform
{
public:
	vector3 position;
	quaternion orientation;
};

class Velocity
{
public:
	vector3 linearVelocity;
	vector3 angularVelocity;
};

struct MeshComponent
{
	Mesh mesh;
	vector3 offset;
};

struct inputControl
{

};

struct CameraLock
{

};

struct DebugVelocity
{
	vector3 prevPos;
};

#endif // COMPONETS_HPP