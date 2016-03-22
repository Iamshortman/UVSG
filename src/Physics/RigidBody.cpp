#include "RigidBody.hpp"
#include "PhysicsWorld.hpp"
#include "World/Entity.hpp"

RigidBody::RigidBody(btCollisionShape* shape, btScalar mass)
{
	btVector3 inertia = btVector3(0, 0, 0);
	if (mass > 0.0f)
	{
		shape->calculateLocalInertia(mass, inertia);
	}

	btDefaultMotionState* motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, motionState, shape, inertia);
	rigidBody = new btRigidBody(boxRigidBodyCI);
}

RigidBody::RigidBody(btCollisionShape* shape, btScalar mass, const btVector3& inertia)
{
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, new btDefaultMotionState(), shape, inertia);
	rigidBody = new btRigidBody(boxRigidBodyCI);
}

void RigidBody::addToPhysicsWorld(PhysicsWorld* physicsWorld, Entity* entity)
{
	//Remove from old physics world
	if (world != nullptr)
	{
		world->removeRigidBody(rigidBody);
	}

	//Assign new Physics
	world = physicsWorld;
	rigidBody->setUserPointer(entity);

	world->addRigidBody(rigidBody);
	setWorldTranform(entity->m_transform);
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

void RigidBody::setWorldTranformUpdate(Transform transform)
{
	btTransform rigidBodyTransform = btTransform(toBtQuat(transform.getOrientation()), toBtVec3(transform.getPos()));
	rigidBody->getMotionState()->setWorldTransform(rigidBodyTransform);
}


PhysicsWorld* RigidBody::getPhysicsWorld() const
{
	return world;
}

RigidBody::~RigidBody()
{	
	if (world != nullptr)
	{
		world->removeRigidBody(rigidBody);
	}
	delete rigidBody->getMotionState();
	delete rigidBody->getCollisionShape();
	delete rigidBody;
}