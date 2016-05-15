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

	int deadzone = 8000;

	int pitchAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_RIGHTY);
	if (pitchAxis > deadzone || pitchAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)pitchAxis) / 32767.0f;
		double angle = -amount * deltaTime * (turnSpeeds.x * 2.0 * M_PI);

		vector3D impulse = parent->m_transform.getOrientation() * vector3D(20.0 * angle, 0, 0);

		parent->m_RigidBody->rigidBody->applyTorqueImpulse(toBtVec3(impulse));
	}

	int yawAxis = -SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_RIGHTX);
	if (yawAxis > deadzone || yawAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)yawAxis) / 32767.0f;
		double angle = amount * deltaTime * (turnSpeeds.y * 2.0 * M_PI);
		
		vector3D impulse = parent->m_transform.getOrientation() * vector3D(0, 20.0 * angle, 0);

		parent->m_RigidBody->rigidBody->applyTorqueImpulse(toBtVec3(impulse));
	}

	int rollAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_LEFTX);
	if (rollAxis > deadzone || rollAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)rollAxis) / 32767.0f;
		double angle = amount * deltaTime * (turnSpeeds.z * 2.0 * M_PI); 

		vector3D impulse = parent->m_transform.getOrientation() * vector3D(0, 0, 20.0 * angle);

		parent->m_RigidBody->rigidBody->applyTorqueImpulse(toBtVec3(impulse));
	}

	parent->m_Velocity.angularVelocity = toGlmVec3(parent->m_RigidBody->rigidBody->getAngularVelocity());

	if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
	{
		parent->m_Velocity.linearVelocity = parent->m_transform.getForward() * 50.0;
	}
}