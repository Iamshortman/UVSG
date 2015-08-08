#ifndef COMPONETS_HPP
#define COMPONETS_HPP

#include "Mesh.hpp"
#include <btBulletDynamicsCommon.h>
#include <entityxInclude.hpp>

class PhysicsWorld;

class RigidBody
{
public:
	RigidBody(PhysicsWorld* physicsWorld, Entity entity, btCollisionShape* shape, btScalar mass);
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
	
	//Getters for the directional vectors.
	const vector3 getForward(){ return orientation * vector3(0.0f, 0.0f, 1.0f); };
	const vector3 getUp(){ return orientation * vector3(0.0f, 1.0f, 0.0f); };
	const vector3 getRight(){ return orientation * vector3(-1.0f, 0.0f, 0.0f); };

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