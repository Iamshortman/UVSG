#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "InputDevice.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;

class InputManager
{
public:
	InputManager();
	~InputManager();

	double getAxis(string name);

	bool getButtonDown(string name);
	bool getButtonPressed(string name);
	bool getButtonDoublePressed(string name);

private:
	vector<InputDevice*> m_devices;
};

#endif //INPUTMANAGER_HPP