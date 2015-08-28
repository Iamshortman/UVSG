#include "CharacterController.hpp"

CharacterController::CharacterController(PhysxWorld* world)
{
	phyxsWorld = world;

	physx::PxCapsuleControllerDesc desc;

	desc.height = 1.8f;
	desc.radius = 0.4f;

	controller = phyxsWorld->controllerManager->createController(desc);
}

CharacterController::~CharacterController()
{

}