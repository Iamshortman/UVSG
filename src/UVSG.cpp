#include <stddef.h>  // defines NULL
#include "UVSG.hpp"

#include "RenderingManager.hpp"

UVSG* UVSG::instance = NULL;

UVSG::UVSG()
{
	entityx::Entity entity = entitySystem.entities.create();
}


void UVSG::update(float timeStep)
{
	//#1 step Physics
	//dynamicsWorld->stepSimulation(timeStep, 4);

	//#2 update Input
	//Update Key Bindings

	//#3 game logic updates

	//#4 audio update

	//#5 rendering
	RenderingManager::getInstance()->update(entitySystem, timeStep);
}

UVSG::~UVSG()
{

}

UVSG* UVSG::getInstance()
{
	if (!instance)
		instance = new UVSG();

	return instance;
}