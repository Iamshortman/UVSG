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
	sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);

	if (!sceneDesc.cpuDispatcher)
	{
		physx::PxDefaultCpuDispatcher* mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = gDefaultFilterShader;

	gScene = gPhysicsSDK->createScene(sceneDesc);
	controllerManager = PxCreateControllerManager(*gScene);


	//Creating PhysX material
	PxMaterial* material = gPhysicsSDK->createMaterial(0.0, 0.0, 0.0);

	//1-Creating static plane	 
	PxTransform planePos = PxTransform(PxVec3(0.0f), PxQuat(physx::PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));	//Position and orientation(transform) for plane actor  
	physx::PxRigidStatic* plane = gPhysicsSDK->createRigidStatic(planePos);								//Creating rigid static actor	
	plane->createShape(physx::PxPlaneGeometry(), *material);												//Defining geometry for plane actor
	gScene->addActor(*plane);																		//Adding plane actor to PhysX scene

}

void PhysxWorld::update(EntityX &entitySystem, double deltaTime)
{
	ComponentHandle<RigidBody> componentRigidBodySearch;
	ComponentHandle<CharacterController> componentCharacterControllerSearch;

	for (Entity entity : entitySystem.entities.entities_with_components(componentRigidBodySearch))
	{
		ComponentHandle<RigidBody> componentRigidBody = entity.component<RigidBody>();

		physx::PxTransform transform = componentRigidBody->physicsBody->getGlobalPose();

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
		componentRigidBody->physicsBody->setGlobalPose(transform, false);

		//Updates the Velocity of the object
		if (entity.has_component<Velocity>())
		{
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();
			componentRigidBody->physicsBody->setLinearVelocity(toPxVec3(componentVelocity->linearVelocity), true);
			componentRigidBody->physicsBody->setAngularVelocity(toPxVec3(componentVelocity->angularVelocity), false);
		}
	}

	for (Entity entity : entitySystem.entities.entities_with_components(componentCharacterControllerSearch))
	{
		ComponentHandle<CharacterController> componentCharacterController = entity.component<CharacterController>();

		//Updates the Tranform of the object
		if (entity.has_component<Transform>())
		{
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			
			componentCharacterController->controller->setPosition(toPxExtendedVec3(componentTransform->getPos()));
			componentCharacterController->controller->setUpDirection(toPxVec3(componentTransform->getUp()));
		}

		if (entity.has_component<Velocity>())
		{
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();

			componentCharacterController->controller->move(toPxVec3(componentVelocity->linearVelocity), 0.01f, deltaTime, nullptr);
		}

	}

	//Simulates the Physics
	physx::PxReal dt = deltaTime;
	if (dt == 0.0)
	{
		//If the delta is too short, set it to some small number.
		dt = 1.0f / 120.0f;
	}

	gScene->simulate(dt);
	gScene->fetchResults(true);

	//Updates the entity with the new info
	for (Entity entity : entitySystem.entities.entities_with_components(componentRigidBodySearch))
	{
		ComponentHandle<RigidBody> componentRigidBody = entity.component<RigidBody>();

		physx::PxTransform transform = componentRigidBody->physicsBody->getGlobalPose();

		//Updates the Tranform of the object
		if (entity.has_component<Transform>())
		{
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			//Position
			componentTransform->position = toGlmVec3(transform.p);
			//Orientation
			componentTransform->orientation = toGlmQuat(transform.q);
		}


		//Updates the Velocity of the object
		if (entity.has_component<Velocity>())
		{
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();
			//Linear Velocity updates
			componentVelocity->linearVelocity = toGlmVec3(componentRigidBody->physicsBody->getLinearVelocity());
			//Angular Velocity updates
			componentVelocity->angularVelocity = toGlmVec3(componentRigidBody->physicsBody->getAngularVelocity());
		}
	}

}

raycastSingleResult PhysxWorld::raycastSingle(vector3 start, vector3 direction, float distance)
{
	raycastSingleResult result;
	result.hasHitEntity = false; //sets this value just incase;

	PxVec3 origin = toPxVec3(start); // [in] Ray origin
	PxVec3 unitDir = toPxVec3(direction).getNormalized(); // [in] Normalized ray direction
	PxReal maxDistance = distance; // [in] Raycast max distance
	physx::PxRaycastBuffer hit; // [out] Raycast results

	// Raycast against all static & dynamic objects (no filtering)
	// The main result from this call is the closest hit, stored in the 'hit.block' structure
	result.hasHit = this->gScene->raycast(origin, unitDir, maxDistance, hit);
	
	if (result.hasHit)
	{
		result.worldHitPos = toGlmVec3(hit.block.position);
		result.worldHitNormal = toGlmVec3(hit.block.normal);

		entityxId* entityId = static_cast<entityxId*>(hit.block.actor->userData);
		if (entityId != nullptr)
		{
			result.hitEntity = UVSG::getInstance()->getEntityFromId(*entityId);
			result.hasHitEntity = true;
		}
	}

	return result;
}


void PhysxWorld::ShutdownPhysX() 
{
	gPhysicsSDK->release();
}

PhysxWorld::~PhysxWorld()
{
	ShutdownPhysX();
}