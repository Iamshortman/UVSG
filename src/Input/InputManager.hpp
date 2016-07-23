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

	//Instance for the Singleton design pattern;
	static InputManager* Instance;

	InputManager();
	~InputManager();

	void update(double deltaTime);

	bool hasAxis(string name);
	double getAxis(string name);

	bool getButtonDown(string name);
	bool getButtonPressed(string name);
	bool getButtonDoublePressed(string name);

private:
	vector<InputDevice*> m_devices;
};

#endif //INPUTMANAGER_HPP