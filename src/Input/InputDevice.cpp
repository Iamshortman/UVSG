#include "InputDevice.hpp"

InputDevice::InputDevice(string name, InputType type, void* devicePtr)
{
	m_name = name;
	m_deviceType = type;
	m_devicePtr = devicePtr;
}

InputDevice::~InputDevice()
{
	for (auto it = this->m_axis.begin(); it != this->m_axis.end(); ++it)
	{
		delete it->second;
		m_axis.erase(it->first);
	}

	for (auto it = this->m_buttons.begin(); it != this->m_buttons.end(); ++it)
	{
		delete it->second;
		m_buttons.erase(it->first);
	}
}

void InputDevice::update(double deltaTime)
{
	for (auto it = this->m_axis.begin(); it != this->m_axis.end(); ++it)
	{
		it->second->update(deltaTime);
	}

	for (auto it = this->m_buttons.begin(); it != this->m_buttons.end(); ++it)
	{
		it->second->update(deltaTime);
	}
}

void InputDevice::addAxis(string name, InputAxis* axis)
{
	if (!hasAxis(name))
	{
		m_axis[name] = axis;
	}
}

void InputDevice::addButton(string name, InputButton* button)
{
	if (!hasButton(name))
	{
		m_buttons[name] = button;
	}
}

bool InputDevice::hasAxis(string name)
{
	return m_axis.find(name) != m_axis.end();
}

double InputDevice::getAxis(string name)
{
	if (hasAxis(name))
	{
		return m_axis[name]->getAxis();
	}

	return 0;
}

bool InputDevice::hasButton(string name)
{
	return m_buttons.find(name) != m_buttons.end();
}

bool InputDevice::getButtonDown(string name)
{
	if (hasButton(name))
	{
		return m_buttons[name]->isButtonDown();
	}

	return false;
}

bool InputDevice::getButtonPressed(string name)
{
	if (hasButton(name))
	{
		return m_buttons[name]->isButtonPressed();
	}

	return false;
}

bool InputDevice::getButtonDoublePressed(string name)
{
	if (hasButton(name))
	{
		return m_buttons[name]->isButtonDoublePressed();
	}

	return false;
}