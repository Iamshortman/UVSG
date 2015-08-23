#ifndef CHARACTERCONTROLLER_HPP
#define CHARACTERCONTROLLER_HPP

#include "entityxInclude.hpp"
#include "physxInclude.hpp"
#include "PhysxWorld.hpp"

class PhysxWorld;

class CharacterController
{
public:
	physx::PxController* controller;

	CharacterController()
	{
		physx::PxCapsuleControllerDesc desc;

		controller = phyxsWorld->controllerManager->createController(desc);
	}

	~CharacterController()
	{

	}

	const PhysxWorld* getPhysxWorld(){ return phyxsWorld; };

private:
	PhysxWorld* phyxsWorld;
};

#endif //CHARACTERCONTROLLER_HPP