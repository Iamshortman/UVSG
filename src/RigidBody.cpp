#include "RigidBody.hpp"
#include "PhysxWorld.hpp"

RigidBody::RigidBody(PhysxWorld* world, Entity& entity, PxGeometry* geometry, PxMaterial* material, float mass)
{
	PxTransform transform;
	if (entity.has_component<Transform>())
	{
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();
		transform.p = toPxVec3(componentTransform->position);
		transform.q = toPxQuat(componentTransform->orientation);
	}

	physicsBody = world->gPhysicsSDK->createRigidDynamic(transform);
	if (mass > 0.0f)
	{
		physicsBody->setMass(mass);
	}

	if (geometry != nullptr)
	{
		PxShape* shape = world->gPhysicsSDK->createShape(*geometry, *material);
		physicsBody->attachShape(*shape);
	}

	world->gScene->addActor(*physicsBody);
	phyxsWorld = world;

	physicsBody->setName("RigidBody");

	//Sets a pointer to a value in mem that the id is stored.
	entityxId* id = new entityxId(entity.id().id());
	physicsBody->userData = (void*)id;
}

StaticBody::StaticBody(PhysxWorld* world, Entity& entity, PxGeometry* geometry, PxMaterial* material)
{
	PxTransform transform;
	if (entity.has_component<Transform>())
	{
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();
		transform.p = toPxVec3(componentTransform->position);
		transform.q = toPxQuat(componentTransform->orientation);
	}

	physicsBody = world->gPhysicsSDK->createRigidStatic(transform);

	if (geometry != nullptr)
	{
		PxShape* shape = world->gPhysicsSDK->createShape(*geometry, *material);
		physicsBody->attachShape(*shape);
	}

	world->gScene->addActor(*physicsBody);
	phyxsWorld = world;

	physicsBody->setName("StaticBody");

	//Sets a pointer to a value in mem that the id is stored.
	entityxId* id = new entityxId(entity.id().id());
	physicsBody->userData = (void*)id;
}
