#include "PhysxWorld.hpp"

PhysxWorld::PhysxWorld()
{
	InitializePhysX();
}

void PhysxWorld::InitializePhysX() 
{
	physx::PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale());

	PxInitExtensions(*gPhysicsSDK);

	// Create the scene
	physx::PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);

	if (!sceneDesc.cpuDispatcher)
	{
		physx::PxDefaultCpuDispatcher* mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = gDefaultFilterShader;

	gScene = gPhysicsSDK->createScene(sceneDesc);

	//Creating PhysX material
	PxMaterial* material = gPhysicsSDK->createMaterial(0.0, 0.0, 0.0);

	//1-Creating static plane	 
	PxTransform planePos = PxTransform(PxVec3(0.0f), PxQuat(physx::PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));	//Position and orientation(transform) for plane actor  
	physx::PxRigidStatic* plane = gPhysicsSDK->createRigidStatic(planePos);								//Creating rigid static actor	
	plane->createShape(physx::PxPlaneGeometry(), *material);												//Defining geometry for plane actor
	gScene->addActor(*plane);																		//Adding plane actor to PhysX scene

}

void PhysxWorld::update(EntityX &entitySystem, float deltaTime)
{
	ComponentHandle<RigidBodyPx> componentRigidBodySearch;
	for (Entity entity : entitySystem.entities.entities_with_components(componentRigidBodySearch))
	{
		ComponentHandle<RigidBodyPx> componentRigidBody = entity.component<RigidBodyPx>();

		physx::PxTransform transform = componentRigidBody->body->getGlobalPose();

		//Updates the Tranform of the object
		if (entity.has_component<Transform>())
		{
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			//Position
			transform.p = toPxVec3(componentTransform->position);
			//Orientation
			transform.q = toPxQuat(componentTransform->orientation);
		}

		//Applies the update
		componentRigidBody->body->setGlobalPose(transform);

		//Updates the Velocity of the object
		if (entity.has_component<Velocity>())
		{
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();
			componentRigidBody->body->setLinearVelocity(toPxVec3(componentVelocity->linearVelocity));
			componentRigidBody->body->setAngularVelocity(toPxVec3(componentVelocity->angularVelocity));
		}
	}

	//Simulates the Physics
	physx::PxReal dt = deltaTime;
	if (dt == 0.0)
	{
		//If the delta is too short, set it to some small number.
		dt = 1.0 / 120.0;
	}

	gScene->simulate(dt);
	gScene->fetchResults(true);

	//Updates the entity with the new info
	for (Entity entity : entitySystem.entities.entities_with_components(componentRigidBodySearch))
	{
		ComponentHandle<RigidBodyPx> componentRigidBody = entity.component<RigidBodyPx>();

		physx::PxTransform transform = componentRigidBody->body->getGlobalPose();

		//Updates the Tranform of the object
		if (entity.has_component<Transform>())
		{
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			//Position
			componentTransform->position = toVector3(transform.p);
			//Orientation
			componentTransform->orientation = toQuaternion(transform.q);
		}


		//Updates the Velocity of the object
		if (entity.has_component<Velocity>())
		{
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();
			//Linear Velocity updates
			componentVelocity->linearVelocity = toVector3(componentRigidBody->body->getLinearVelocity());
			//Angular Velocity updates
			componentVelocity->angularVelocity = toVector3(componentRigidBody->body->getAngularVelocity());
		}
	}

}


void PhysxWorld::ShutdownPhysX() 
{
	gPhysicsSDK->release();
}

PhysxWorld::~PhysxWorld()
{
	ShutdownPhysX();
}