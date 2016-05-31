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

void RigidBody::addToPhysicsWorld(PhysicsWorld* physicsWorld, Entity* entity, Transform worldTransform)
{
	//Remove from old physics world
	if (world != nullptr)
	{
		world->removeRigidBody(rigidBody);
	}

	//Assign new Physics
	world = physicsWorld;
	rigidBody->setUserPointer(entity);

	setWorldTransform(worldTransform);

	world->addRigidBody(rigidBody);
}

void RigidBody::Activate(bool activate)
{
	rigidBody->activate(activate);
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
	btTransform bulletTransform = rigidBody->getCenterOfMassTransform();

	transform.setPos( toGlmVec3(bulletTransform.getOrigin()) );
	transform.setOrientation(toGlmQuat(bulletTransform.getRotation()));

	return transform;
}

void RigidBody::setWorldTransform(Transform transform)
{
	btTransform rigidBodyTransform = btTransform( toBtQuat(transform.getOrientation()), toBtVec3(transform.getPos()) );
	rigidBody->setCenterOfMassTransform(rigidBodyTransform);
}

vector3D RigidBody::getLinearVelocity() const
{
	return toGlmVec3(rigidBody->getLinearVelocity());
}

void RigidBody::setLinearVelocity(vector3D velocity)
{
	rigidBody->setLinearVelocity(toBtVec3(velocity));
}

vector3D RigidBody::getAngularVelocity() const
{
	return toGlmVec3(rigidBody->getAngularVelocity());
}

void RigidBody::setAngularVelocity(vector3D velocity)
{
	rigidBody->setAngularVelocity(toBtVec3(velocity));
}


Transform RigidBody::getCOMTransform()
{
	Transform transform;
	btTransform bulletTransform = ((btDefaultMotionState*)rigidBody->getMotionState())->m_centerOfMassOffset;

	transform.setPos(toGlmVec3(bulletTransform.getOrigin()));
	transform.setOrientation(toGlmQuat(bulletTransform.getRotation()));

	return transform;
}

void RigidBody::setCOMTransform(Transform transform)
{
	btTransform COMTransform = btTransform(toBtQuat(transform.getOrientation()), toBtVec3(transform.getPos()));
	btDefaultMotionState* state = (btDefaultMotionState*)rigidBody->getMotionState();
	state->m_centerOfMassOffset = COMTransform;
}

void RigidBody::applyCentralForce(vector3D force)
{
	rigidBody->applyCentralForce(toBtVec3(force));
}

void RigidBody::applyCentralImpulse(vector3D impulse)
{
	rigidBody->applyCentralImpulse(toBtVec3(impulse));
}

void RigidBody::applyTorque(vector3D torque)
{
	rigidBody->applyTorque(toBtVec3(torque));
}

void RigidBody::applyTorqueImpulse(vector3D torque)
{
	rigidBody->applyTorqueImpulse(toBtVec3(torque));
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