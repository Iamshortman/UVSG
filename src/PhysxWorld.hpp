#ifndef PHYSXWORLD_HPP
#define PHYSXWORLD_HPP

#include <entityxInclude.hpp>
#include "physxInclude.hpp"
#include "Components.hpp"
#include "PxRigidBody.hpp"
#include "UVSG.hpp"

using namespace std;

struct raycastSingleResult
{
	bool hasHit;
	bool hasHitEntity;
	vector3 worldHitPos;
	vector3 worldHitNormal;
	Entity hitEntity;
};

class PhysxWorld
{
public:
	PhysxWorld();
	~PhysxWorld();
	
	void update(EntityX &ex, double deltaTime);
	void InitializePhysX();
	void ShutdownPhysX();

	raycastSingleResult raycastSingle(vector3 start, vector3 direction, float distance);

	physx::PxPhysics* gPhysicsSDK;
	physx::PxScene* gScene = NULL;

private:
	physx::PxDefaultErrorCallback gDefaultErrorCallback;
	physx::PxDefaultAllocator gDefaultAllocatorCallback;
	physx::PxSimulationFilterShader gDefaultFilterShader = physx::PxDefaultSimulationFilterShader;
};


#endif //PHYSXWORLD_HPP