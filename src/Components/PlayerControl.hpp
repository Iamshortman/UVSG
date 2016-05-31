#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include "Util.hpp"
#include "Components/Component.hpp"
#include "SDL2/SDL_gamecontroller.h"

struct PlayerControl : public Component
{
	PlayerControl(double linear, double angular, SDL_GameController* controllerToUse)
	{
		linearSpeed = linear;
		angularSpeed = angular;
		controller = controllerToUse;
	};

	double linearSpeed;

	//rad per second
	double angularSpeed;
	SDL_GameController* controller;

	virtual void update(double deltaTime);
};

#endif //PLAYERCONTROL_HPP
