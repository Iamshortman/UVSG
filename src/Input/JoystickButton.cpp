#include "JoystickButton.hpp"

JoystickButton::JoystickButton(SDL_Joystick* device, int button)
{
	m_device = device;
	m_button = button;
}

void JoystickButton::update(double deltaTime)
{
	//Make new state the old state
	m_buttonValuePrev = m_buttonValue;

	m_buttonValue = SDL_JoystickGetButton(m_device, m_button);
}

bool JoystickButton::isButtonDown()
{
	if (m_buttonValue)
	{
		return true;
	}

	return false;
}

bool JoystickButton::isButtonPressed()
{
	if (m_buttonValue && !m_buttonValuePrev)
	{
		return true;
	}

	return false;
}

bool JoystickButton::isButtonDoublePressed()
{
	return false;
}
