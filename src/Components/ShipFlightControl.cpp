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

vector3D Lerp(vector3D start, vector3D end, double percent)
{
	return (start + percent*(end - start));
}

void ShipFlightControl::update(double deltaTime)
{
	Entity* parent = getParent();
	if (parent == nullptr)
	{
		printf("No parent!!!\n");
		exit(1);
	}

	Transform transform = parent->getTransform();

	int deadzone = 8000;


	int pitchAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_RIGHTY);
	if (pitchAxis > deadzone || pitchAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)pitchAxis) / 32767.0;

		parent->applyTorque(transform.getRight() * (amount * 2000.0));
	}

	int yawAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_RIGHTX);
	if (yawAxis > deadzone || yawAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)yawAxis) / 32767.0;

		parent->applyTorque(transform.getUp() * (-amount * 2000.0));
	}

	int rollAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_LEFTX);
	if (rollAxis > deadzone || rollAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)rollAxis) / 32767.0;

		parent->applyTorque(transform.getForward()  * (amount * 10000.0));
	}
	
	double throttle = 0.0;
	if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
		throttle -= 1.0;
	if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
		throttle += 1.0;

	if (throttle != 0.0)
	{
		parent->applyCentralImpulse(transform.getForward() * throttle * 250.0);
	}

	if (!SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
	{
		//Dampening
		parent->setAngularVelocity(Lerp(parent->getAngularVelocity(), vector3D(0.0), deltaTime * 2.0));
		parent->setLinearVelocity(Lerp(parent->getLinearVelocity(), vector3D(0.0), deltaTime));
	}
}