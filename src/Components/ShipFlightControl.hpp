#ifndef SHIPFLIGHTCONTROL_HPP
#define SHIPFLIGHTCONTROL_HPP

#include "Components/Component.hpp"
#include "SDL2/SDL_joystick.h"
#include "glmInclude.hpp"

class ShipFlightControl : public Component
{
public:
	ShipFlightControl(SDL_Joystick* joystick);
	virtual ~ShipFlightControl();

	virtual void update(double deltaTime);

private:
	SDL_Joystick* m_joystick = nullptr;
};

#endif //SHIPFLIGHTCONTROL_HPP