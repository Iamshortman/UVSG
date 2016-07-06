#ifndef JOYSTICKBUTTON_HPP
#define JOYSTICKBUTTON_HPP

#include "InputButton.hpp"
#include <SDL2\SDL_joystick.h>

class JoystickButton: public InputButton
{
public:
	JoystickButton(SDL_Joystick* device, int button);
	virtual void update(double deltaTime);
	virtual bool isButtonDown();
	virtual bool isButtonPressed();
	virtual bool isButtonDoublePressed();

private:
	SDL_Joystick* m_device = nullptr;
	int m_button = 0;

	Uint8 m_buttonValue = 0;
	Uint8 m_buttonValuePrev = 0;
};

#endif //JOYSTICKBUTTON_HPP