#include "KeyboardButton.hpp"

#include "GamepadButton.hpp"

KeyboardButton::KeyboardButton(int button)
{
	m_button = button;
}

void KeyboardButton::update(double deltaTime)
{
	//Make new state the old state
	m_buttonValuePrev = m_buttonValue;

	m_buttonValue = false;
}

bool KeyboardButton::isButtonDown()
{
	if (m_buttonValue)
	{
		return true;
	}

	return false;
}

bool KeyboardButton::isButtonPressed()
{
	if (m_buttonValue && !m_buttonValuePrev)
	{
		return true;
	}

	return false;
}

bool KeyboardButton::isButtonDoublePressed()
{
	return false;
}


