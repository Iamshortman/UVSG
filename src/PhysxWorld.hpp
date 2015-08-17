#ifndef PHYSXWORLD_HPP
#define PHYSXWORLD_HPP

#include <entityxInclude.hpp>
#include "physxInclude.hpp"
#include "Components.hpp"
#include "PxRigidBody.hpp"

using namespace std;

class PhysxWorld
{
public:
	PhysxWorld();
	~PhysxWorld();
	
	void update(EntityX &ex, float deltaTime);
	void InitializePhysX();
	void ShutdownPhysX();
	physx::PxPhysics* gPhysicsSDK;
	physx::PxScene* gScene = NULL;

private:
	physx::PxDefaultErrorCallback gDefaultErrorCallback;
	physx::PxDefaultAllocator gDefaultAllocatorCallback;
	physx::PxSimulationFilterShader gDefaultFilterShader = physx::PxDefaultSimulationFilterShader;
};


#endif //PHYSXWORLD_HPP