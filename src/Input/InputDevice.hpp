#ifndef INPUTDEVICE_HPP
#define INPUTDEVICE_HPP

#include "InputAxis.hpp"
#include "InputButton.hpp"

#include <string>
#include <hash_map>

using std::string;
using std::hash_map;

enum InputType
{
	KEYBOARD_MOUSE,
	JOYSTICK,
	GAMEPAD
};

class InputDevice
{
public:
	string m_name;
	InputType m_deviceType;
	void* m_devicePtr  = nullptr;


	InputDevice(string name, InputType type, void* devicePtr);
	~InputDevice();

	void update(double deltaTime);

	void addAxis(string name, InputAxis* axis);
	void addButton(string name, InputButton* button);

	bool hasAxis(string name);
	double getAxis(string name);

	bool hasButton(string name);
	bool getButtonDown(string name);
	bool getButtonPressed(string name);
	bool getButtonDoublePressed(string name);

private:
	hash_map<string, InputAxis*> m_axis;
	hash_map<string, InputButton*> m_buttons;
};

#endif //INPUTDEVICE_HPP