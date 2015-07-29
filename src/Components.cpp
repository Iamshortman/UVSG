#include "Components.hpp"
#include "PhysicsWorld.hpp"

RigidBody::RigidBody(PhysicsWorld* physicsWorld, entityx::Entity entity, btCollisionShape* shape, btScalar mass)
{
	this->world = physicsWorld;

	btVector3 Inertia;
	shape->calculateLocalInertia(mass, Inertia);
	motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, motionState, shape, Inertia);
	rigidBody = new btRigidBody(boxRigidBodyCI);
	collisionShape = shape;
	this->world->addRigidBody(rigidBody);
}

RigidBody::~RigidBody()
{
	world->removeRigidBody(rigidBody);
	delete rigidBody->getMotionState();
	delete rigidBody->getCollisionShape();
	delete rigidBody;
}