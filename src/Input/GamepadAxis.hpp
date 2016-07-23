#ifndef GAMEPADAXIS_HPP
#define GAMEPADAXIS_HPP

#include "InputAxis.hpp"
#include <SDL2\SDL_GameController.h>

class GamepadAxis : public InputAxis
{
public:
	GamepadAxis(SDL_GameController* device, SDL_GameControllerAxis axis, double deadzone, bool inverted);
	virtual void update(double deltaTime);
	virtual double getAxis();

	virtual void setDeadzone(double deadzone);
	virtual void setInverted(bool inverted);

	virtual double getDeadzone();
	virtual bool getInverted();

private:
	SDL_GameController* m_device = nullptr;
	SDL_GameControllerAxis m_axis;
	double m_deadzone = 0.0;
	bool m_inverted = false;

	int m_axisValue = 0;
};

#endif //GAMEPADAXIS_HPP