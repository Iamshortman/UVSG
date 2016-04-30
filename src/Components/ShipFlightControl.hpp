#ifndef SHIPFLIGHTCONTROL_HPP
#define SHIPFLIGHTCONTROL_HPP

#include "Components/Component.hpp"
#include "SDL2/SDL_gamecontroller.h"
#include "glmInclude.hpp"

class ShipFlightControl : public Component
{
public:
	ShipFlightControl(SDL_GameController* controllerToUse);
	virtual ~ShipFlightControl();

	virtual void update(double deltaTime);

private:
	SDL_GameController* m_controller = nullptr;

	//Flight Data
	double m_throttle = 0.0; //Forward Back Throttle
	double m_upDownThrottle = 0.0;
	double m_leftRightThrottle = 0.0;

	//In Rotations Per Second
	vector3D turnSpeeds;
};

#endif //SHIPFLIGHTCONTROL_HPP