#include "PxRigidBody.hpp"
#include "PhysxWorld.hpp"

RigidBodyPx::RigidBodyPx(PhysxWorld* world, Entity& entity, PxGeometry* shape, PxMaterial* material, float mass)
{
	PxTransform transform;
	if (entity.has_component<Transform>())
	{
		ComponentHandle<Transform> componentTransform = entity.component<Transform>();
		transform.p = toPxVec3(componentTransform->position);
		transform.q = toPxQuat(componentTransform->orientation);
	}

	body = PxCreateDynamic(*world->gPhysicsSDK, transform, *shape, *material, mass);
	world->gScene->addActor(*body);
	phyxsWorld = world;

	//Sets a pointer to a value in mem that the id is stored.
	entityxId* id = new entityxId(entity.id().id());
	body->userData = (void*)id;
}