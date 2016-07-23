#include "GamepadAxis.hpp"

#include <math.h>

GamepadAxis::GamepadAxis(SDL_GameController* device, SDL_GameControllerAxis axis, double deadzone, bool inverted)
{
	m_device = device;
	m_axis = axis;
	m_deadzone = deadzone;
	m_inverted = inverted;
}

void GamepadAxis::update(double deltaTime)
{
	m_axisValue = SDL_GameControllerGetAxis(m_device, m_axis);

	//Invert axis
	if (m_inverted)
	{
		m_axisValue *= -1;
	}
}

double GamepadAxis::getAxis()
{
	double range = 1.0 - m_deadzone;

	//Get between -1 and 1
	double value = ((double)m_axisValue) / 32767.0;

	//apply deadzone
	if (abs(value) > m_deadzone)
	{
		double sign = value / abs(value);
		value = abs(value) - m_deadzone;
		value /= range;

		value *= sign;

		return value;
	}

	return 0.0;
}

void GamepadAxis::setDeadzone(double deadzone)
{
	m_deadzone = deadzone;
}

void GamepadAxis::setInverted(bool inverted)
{
	m_inverted = inverted;
}

double GamepadAxis::getDeadzone()
{
	return m_deadzone;
}

bool GamepadAxis::getInverted()
{
	return m_inverted;
}