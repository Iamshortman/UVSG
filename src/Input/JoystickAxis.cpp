#include "JoystickAxis.hpp"

#include <math.h>

JoystickAxis::JoystickAxis(SDL_Joystick* device, int axis, double deadzone, bool inverted)
{
	m_device = device;
	m_axis = axis;
	m_deadzone = deadzone;
	m_inverted = inverted;
}

void JoystickAxis::update(double deltaTime)
{
	m_axisValue = SDL_JoystickGetAxis(m_device, m_axis);

	//Invert axis
	if (m_inverted)
	{
		m_axisValue *= -1;
	}
}

double JoystickAxis::getAxis()
{
	//Get between -1 and 1
	double value = ((double)m_axisValue) / 32767.0;

	//apply deadzone
	if (abs(value) > m_deadzone)
	{
		return value;
	}

	return 0.0;
}

void JoystickAxis::setDeadzone(double deadzone)
{
	m_deadzone = deadzone;
}

void JoystickAxis::setInverted(bool inverted)
{
	m_inverted = inverted;
}

double JoystickAxis::getDeadzone()
{
	return m_deadzone;
}

bool JoystickAxis::getInverted()
{
	return m_inverted;
}