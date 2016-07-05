#ifndef JOYSTICKAXIS_HPP
#define JOYSTICKAXIS_HPP

#include "InputAxis.hpp"
#include <SDL2\SDL_joystick.h>

class JoystickAxis
{
public:
	JoystickAxis(SDL_Joystick* device, int axisId, double deadzone);
	virtual void update(double deltaTime);
	virtual double getAxis();
	virtual void setDeadzone(double deadzone);

private:
	SDL_Joystick* m_device = nullptr;
	int m_axisId;
	double m_deadzone;
};

#endif //JOYSTICKAXIS_HPP