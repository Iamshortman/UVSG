#include <stddef.h>  // defines NULL
#include "RenderingManager.hpp"

RenderingManager* RenderingManager::instance = NULL;

RenderingManager::RenderingManager()
{

}

void RenderingManager::update(entityx::EntityX ex, float timeStep)
{

}

RenderingManager::~RenderingManager()
{

}

RenderingManager* RenderingManager::getInstance()
{
	if (!instance)
		instance = new RenderingManager();

	return instance;
}