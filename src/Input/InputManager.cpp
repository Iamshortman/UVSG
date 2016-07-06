#include "InputManager.hpp"

#include <SDL2/SDL.h>
#include "JoystickAxis.hpp"
#include "JoystickButton.hpp"

InputManager* InputManager::Instance = nullptr;

InputManager::InputManager()
{
	int num_joy = SDL_NumJoysticks();
	printf("\n%i joystick(s) found.\n", num_joy);
	for (int i = 0; i < num_joy; i++)
	{
		if (!SDL_IsGameController(i))
		{
			SDL_Joystick* joystick = SDL_JoystickOpen(i);
			string name = SDL_JoystickName(joystick);
			printf("%s \n", name.c_str());

			InputDevice* device = new InputDevice(name);
			this->m_devices.push_back(device);

			if (name == "Logitech Extreme 3D")
			{
				device->addAxis("ship_roll", new JoystickAxis(joystick, 0, 0.2, false));
				device->addAxis("ship_pitch", new JoystickAxis(joystick, 1, 0.2, true));
				device->addAxis("ship_yaw", new JoystickAxis(joystick, 2, 0.2, true));
				device->addAxis("ship_throttle", new JoystickAxis(joystick, 3, 0.0, true));
				device->addButton("ship_flight_assist", new JoystickButton(joystick, 2));
			}

		}
	}


}

InputManager::~InputManager()
{
	for (InputDevice* device : m_devices)
	{
		delete device;
	}
}

void InputManager::update(double deltaTime)
{
	for (InputDevice* device : m_devices)
	{
		device->update(deltaTime);
	}
}

//TODO return axis for last changed instead?????
/*
	name: the axis name;
	return: the highest axis value(ignoring sign) of all devices; 
*/
double InputManager::getAxis(string name)
{
	double axisValue = 0.0;
	double tempAxisValue = 0.0;

	for (InputDevice* device : m_devices)
	{
		if (device->hasAxis(name))
		{
			tempAxisValue = device->getAxis(name);
			if (abs(axisValue) < abs(tempAxisValue))
			{
				axisValue = tempAxisValue;
			}
		}
	}

	return axisValue;
}

/*
	name: the button name;
	return: true if any devices have the button down;
*/
bool InputManager::getButtonDown(string name)
{
	for (InputDevice* device : m_devices)
	{
		if (device->hasButton(name) && device->getButtonDown(name))
		{
			return true;
		}
	}

	return false;
}

/*
	name: the button name;
	return: true if any devices have the button pressed;
*/
bool InputManager::getButtonPressed(string name)
{
	for (InputDevice* device : m_devices)
	{
		if (device->hasButton(name) && device->getButtonPressed(name))
		{
			return true;
		}
	}

	return false;
}

/*
	name: the button name;
	return: true if any devices have the button double pressed;
*/
bool InputManager::getButtonDoublePressed(string name)
{
	for (InputDevice* device : m_devices)
	{
		if (device->hasButton(name) && device->getButtonDoublePressed(name))
		{
			return true;
		}
	}

	return false;
}