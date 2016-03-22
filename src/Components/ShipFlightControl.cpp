#include "Components/ShipFlightControl.hpp"
#include "World/Entity.hpp"
#include <stdio.h>
#include <stdlib.h> 
#include "Util.hpp"

ShipFlightControl::ShipFlightControl(SDL_Joystick* joystick)
{
	m_joystick = joystick;
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

	if (parent->m_RigidBody != nullptr)
	{
		int throttleAxis = SDL_JoystickGetAxis(m_joystick, 3);

		double amount = -((double)throttleAxis) / 32767.0;
		amount += 1.0f;
		amount /= 2.0f;

		if (SDL_JoystickGetButton(m_joystick, 0))
		{
			amount *= 10000.0;
		}

		parent->m_Velocity.linearVelocity = parent->m_transform.getForward() * amount * 300.0;

		int deadzone = 16000;
		parent->m_Velocity.angularVelocity = vector3D(0.0);


		int pitchAxis = SDL_JoystickGetAxis(m_joystick, 1);
		if (pitchAxis > deadzone || pitchAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = ((double)pitchAxis) / 32767.0f;
			double angle = amount * deltaTime * 80.0f;
			quaternionD pitchQuat = glm::normalize(glm::angleAxis(angle, parent->m_transform.getRight()));

			parent->m_Velocity.angularVelocity += glm::eulerAngles(pitchQuat);
		}

		int yawAxis = SDL_JoystickGetAxis(m_joystick, 2);
		if (yawAxis > deadzone || yawAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = -((double)yawAxis) / 32767.0f;
			double angle = amount * deltaTime * 60.0f;
			quaternionD yawQuat = glm::normalize(glm::angleAxis(angle, parent->m_transform.getUp()));

			parent->m_Velocity.angularVelocity += glm::eulerAngles(yawQuat);
		}

		int rollAxis = SDL_JoystickGetAxis(m_joystick, 0);

		if (SDL_JoystickGetButton(m_joystick, 1))
		{
			rollAxis = 32766;
		}

		if (rollAxis > deadzone || rollAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = ((double)rollAxis) / 32767.0f;
			double angle = amount * deltaTime * 120.0f;
			quaternionD rollQuat = glm::normalize(glm::angleAxis(angle, parent->m_transform.getForward()));

			parent->m_Velocity.angularVelocity += glm::eulerAngles(rollQuat);
		}

	}

}