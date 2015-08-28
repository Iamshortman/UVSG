#ifndef CHARACTERCONTROLLER_HPP
#define CHARACTERCONTROLLER_HPP

#include "entityxInclude.hpp"
#include "physxInclude.hpp"
#include "PhysxWorld.hpp"

//Prototype Class for the world.
class PhysxWorld;

class CharacterController
{
public:
	physx::PxController* controller;

	CharacterController(PhysxWorld* world);

	~CharacterController();

	const PhysxWorld* getPhysxWorld(){ return phyxsWorld; };

private:
	PhysxWorld* phyxsWorld;
};

#endif //CHARACTERCONTROLLER_HPP