#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "InputDevice.hpp"

#include <string>
#include <vector>
#include <SDL2/SDL_gamecontroller.h>

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
	void loadController(SDL_GameController* controller);
	void unloadController(SDL_GameController* controller);

	bool hasAxis(string name);
	double getAxis(string name);

	bool getButtonDown(string name);
	bool getButtonPressed(string name);
	bool getButtonDoublePressed(string name);

private:
	vector<InputDevice*> m_devices;
};

#endif //INPUTMANAGER_HPP