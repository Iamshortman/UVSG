#include "RigidBody.hpp"
#include "PhysicsWorld.hpp"

RigidBody::RigidBody(PhysicsWorld* physicsWorld, btCollisionShape* shape, btScalar mass)
{
	world = physicsWorld;

	btVector3 Inertia = btVector3(0, 0, 0);
	if (mass > 0.0f)
	{
		shape->calculateLocalInertia(mass, Inertia);
	}

	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, new btDefaultMotionState(), shape, Inertia);
	rigidBody = new btRigidBody(boxRigidBodyCI);
	world->addRigidBody(rigidBody);
}

//Not sure this actually works.....
void RigidBody::setCollisionShape(bool deleteOldShape, btCollisionShape* newShape)
{
	world->removeRigidBody(rigidBody);

	if (deleteOldShape)
	{
		delete rigidBody->getCollisionShape();
	}

	rigidBody->setCollisionShape(newShape);

	world->addRigidBody(rigidBody);
}

Transform RigidBody::getWorldTransform()
{
	Transform transform;
	btTransform bulletTransform;
	rigidBody->getMotionState()->getWorldTransform(bulletTransform);

	transform.setPos( toGlmVec3(bulletTransform.getOrigin()) );
	transform.setOrientation(toGlmQuat(bulletTransform.getRotation()));

	return transform;
}

void RigidBody::setWorldTranform(Transform transform)
{
	btTransform rigidBodyTransform = btTransform( toBtQuat(transform.getOrientation()), toBtVec3(transform.getPos()) );
	rigidBody->setWorldTransform(rigidBodyTransform);
}

RigidBody::~RigidBody()
{
	world->removeRigidBody(rigidBody);
	delete rigidBody->getMotionState();
	delete rigidBody->getCollisionShape();
	delete rigidBody;
}