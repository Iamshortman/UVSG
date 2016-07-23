#include "GamepadButton.hpp"

GamepadButton::GamepadButton(SDL_GameController* device, SDL_GameControllerButton button)
{
	m_device = device;
	m_button = button;
}

void GamepadButton::update(double deltaTime)
{
	//Make new state the old state
	m_buttonValuePrev = m_buttonValue;

	m_buttonValue = SDL_GameControllerGetButton(m_device, m_button);
}

bool GamepadButton::isButtonDown()
{
	if (m_buttonValue)
	{
		return true;
	}

	return false;
}

bool GamepadButton::isButtonPressed()
{
	if (m_buttonValue && !m_buttonValuePrev)
	{
		return true;
	}

	return false;
}

bool GamepadButton::isButtonDoublePressed()
{
	return false;
}


