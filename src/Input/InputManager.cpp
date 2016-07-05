#include "InputManager.hpp"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

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
	//for (InputDevice* device : m_devices)
	//{
	//	if (device->hasAxis(name) && device->getButtonDown(name))
	//	{
	//		return true;
	//	}
	//}

	return false;
}

/*
	name: the button name;
	return: true if any devices have the button pressed;
*/
bool InputManager::getButtonPressed(string name)
{
	//for (InputDevice* device : m_devices)
	//{
	//	if (device->hasAxis(name) && device->getButtonPressed(name))
	//	{
	//		return true;
	//	}
	//}

	return false;
}

/*
	name: the button name;
	return: true if any devices have the button double pressed;
*/
bool InputManager::getButtonDoublePressed(string name)
{
	//for (InputDevice* device : m_devices)
	//{
	//	if (device->hasAxis(name) && device->getButtonDoublePressed(name))
	//	{
	//		return true;
	//	}
	//}

	return false;
}