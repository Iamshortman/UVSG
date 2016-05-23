#include "Components/ShipFlightControl.hpp"
#include "World/Entity.hpp"
#include <stdio.h>
#include <stdlib.h> 
#include "Util.hpp"

#include "Ship/Directions.hpp"

ShipFlightControl::ShipFlightControl(SDL_GameController* controllerToUse)
{
	m_controller = controllerToUse;
	turnSpeeds = vector3D(0.5, 0.5, 0.75);
}

ShipFlightControl::~ShipFlightControl()
{

}

void ShipFlightControl::update(double deltaTime)
{
	Entity* parent = getParent();
	if (parent == nullptr)
	{
		printf("No parent!!!\n");
		exit(1);
	}

}