#include "InputManager.hpp"

#include <SDL2/SDL.h>
#include "JoystickAxis.hpp"
#include "JoystickButton.hpp"
#include "GamepadAxis.hpp"
#include "GamepadButton.hpp"

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

			InputDevice* device = new InputDevice(name, InputType::JOYSTICK, joystick);
			this->m_devices.push_back(device);

			if (name == "Logitech Extreme 3D")
			{
				device->addAxis("ship_roll", new JoystickAxis(joystick, 0, 0.2, false));
				device->addAxis("ship_pitch", new JoystickAxis(joystick, 1, 0.2, true));
				device->addAxis("ship_yaw", new JoystickAxis(joystick, 2, 0.2, true));

				//device->addAxis("ship_throttle", new JoystickAxis(joystick, 3, 0.0, true));

				device->addButton("ship_flight_assist", new JoystickButton(joystick, 2));
			}
			
			if (name == "CH PRO THROTTLE USB ")
			{
				device->addAxis("ship_throttle", new JoystickAxis(joystick, 2, 0.0, true));
			}
		}
		else
		{
			SDL_GameController* controller = SDL_GameControllerOpen(i);
			string name = SDL_GameControllerName(controller);
			printf("%s \n", name.c_str());

			InputDevice* device = new InputDevice(name, InputType::GAMEPAD, controller);
			this->m_devices.push_back(device);

			//Ship Controls
			device->addAxis("ship_roll", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_LEFTX, 0.2, false));
			device->addAxis("ship_pitch", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY, 0.2, true));
			device->addAxis("ship_yaw", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX, 0.2, true));

			device->addButton("ship_throttle_up", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
			device->addButton("ship_throttle_down", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
			device->addButton("ship_flight_assist", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_A));
			device->addButton("ship_eject", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_BACK));

			//Editor Controls
			device->addAxis("editor_cell_forward_back", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_LEFTY, 0.4, true));
			device->addAxis("editor_cell_left_right", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_LEFTX, 0.4, true));

			device->addButton("editor_cell_up", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
			device->addButton("editor_cell_down", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
			device->addButton("editor_cell_place", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_A));
			device->addButton("editor_cell_next", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP));
			device->addButton("editor_cell_prev", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN));

			device->addAxis("editor_camera_pitch", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY, 0.2, false));
			device->addAxis("editor_camera_yaw", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX, 0.2, false));
			device->addAxis("editor_camera_zoom_in", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT, 0.1, false));
			device->addAxis("editor_camera_zoom_out", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX, 0.1, false));

			device->addButton("editor_exit", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_BACK));


			//Player Controls
			device->addAxis("player_pitch", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY, 0.2, false));
			device->addAxis("player_yaw", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX, 0.2, true));
			device->addAxis("player_forward", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_LEFTY, 0.2, false));
			device->addAxis("player_strafe", new GamepadAxis(controller, SDL_CONTROLLER_AXIS_LEFTX, 0.2, false));

			device->addButton("player_fast", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
			device->addButton("player_click", new GamepadButton(controller, SDL_CONTROLLER_BUTTON_A));
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

bool InputManager::hasAxis(string name)
{
	for (InputDevice* device : m_devices)
	{
		if (device->hasAxis(name))
		{
			return true;
		}
	}

	return false;
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