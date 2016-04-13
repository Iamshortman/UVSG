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

/*class PlayerControlSystem : public System < PlayerControlSystem >
{
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override
	{
		double timestep = ((double)dt);
		SDL_GameController* controller = UVSG::getInstance()->controller;
		SDL_Joystick* joystick = UVSG::getInstance()->joystick;

		//For all entities with the Player Control and Tranform components.
		ComponentHandle<PlayerControl> PlayerControlSearch;
		ComponentHandle<Transform> transformComponentSearch;
		for (Entity entity : es.entities_with_components(PlayerControlSearch, transformComponentSearch))
		{
			//Get the component
			ComponentHandle<PlayerControl> PlayerControl = entity.component<PlayerControl>();
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();

			int deadzone = 8000;

			int pitchAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY); //SDL_JoystickGetAxis(joystick, 5);

			if (pitchAxis > deadzone || pitchAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)pitchAxis) / 32767.0f;
				double angle = amount * timestep * this->angularSpeed;

				//Negitive angle because the joystick layout is backwards
				quaternionD pitchQuat = glm::normalize(glm::angleAxis( -angle, parent->m_transform.getRight() ));

				parent->m_transform.m_orientation = pitchQuat * parent->m_transform.m_orientation;
			}

			int yawAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

			if (yawAxis > deadzone || yawAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)yawAxis) / 32767.0f;
				double angle = amount * timestep * this->angularSpeed;

				quaternionD yawQuat = glm::normalize(glm::angleAxis( -angle, vector3D(0.0f, 1.0f, 0.0f) ));

				parent->m_transform.m_orientation = yawQuat * parent->m_transform.m_orientation;
			}


			int forwardAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY); //SDL_JoystickGetAxis(joystick, 1);

			if (forwardAxis > deadzone || forwardAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)forwardAxis) / 32767.0f;
				double distance = amount * timestep * this->linearSpeed;
				parent->m_transform.m_position += parent->m_transform.getForward() * -distance;
			}

			int strafeAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);

			if (strafeAxis > deadzone || strafeAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)strafeAxis) / 32767.0f;
				double distance = amount * timestep * this->linearSpeed;
				parent->m_transform.m_position += parent->m_transform.getRight() * distance;
			}

			static int lastButton = 0;

			int button = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
			if (button && !lastButton)
			{
				double rayDistance = 1000.0f;
				vector3D startPos = parent->m_transform.getPos();
				vector3D endPos = parent->m_transform.getPos() + (parent->m_transform.getForward() * rayDistance);
				SingleRayTestResult result = UVSG::getInstance()->physicsWorld->singleRayTest(startPos, endPos);

				if (result.hasHit)
				{
				}

			}
			lastButton = button;

		}
	};
};*/

#endif //PLAYERCONTROL_HPP
