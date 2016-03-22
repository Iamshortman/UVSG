#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include "EntityxInclude.hpp"

#include "Util.hpp"

struct PlayerControlComponent
{
	PlayerControlComponent(double linear, double angular)
	{
		linearSpeed = linear;
		angularSpeed = angular;
	};

	double linearSpeed;

	//rad per second
	double angularSpeed;
};

/*class PlayerControlSystem : public System < PlayerControlSystem >
{
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override
	{
		double timestep = ((double)dt);
		SDL_GameController* controller = UVSG::getInstance()->controller;
		SDL_Joystick* joystick = UVSG::getInstance()->joystick;

		//For all entities with the Player Control and Tranform components.
		ComponentHandle<PlayerControlComponent> playerControlComponentSearch;
		ComponentHandle<Transform> transformComponentSearch;
		for (Entity entity : es.entities_with_components(playerControlComponentSearch, transformComponentSearch))
		{
			//Get the component
			ComponentHandle<PlayerControlComponent> playerControlComponent = entity.component<PlayerControlComponent>();
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();

			int deadzone = 8000;

			int pitchAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY); //SDL_JoystickGetAxis(joystick, 5);

			if (pitchAxis > deadzone || pitchAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)pitchAxis) / 32767.0f;
				double angle = amount * timestep * playerControlComponent->angularSpeed;

				//Negitive angle because the joystick layout is backwards
				quaternionD pitchQuat = glm::normalize(glm::angleAxis( -angle, componentTransform->getRight() ));

				componentTransform->m_orientation = pitchQuat * componentTransform->m_orientation;
			}

			int yawAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

			if (yawAxis > deadzone || yawAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)yawAxis) / 32767.0f;
				double angle = amount * timestep * playerControlComponent->angularSpeed;

				quaternionD yawQuat = glm::normalize(glm::angleAxis( -angle, vector3D(0.0f, 1.0f, 0.0f) ));

				componentTransform->m_orientation = yawQuat * componentTransform->m_orientation;
			}


			int forwardAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY); //SDL_JoystickGetAxis(joystick, 1);

			if (forwardAxis > deadzone || forwardAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)forwardAxis) / 32767.0f;
				double distance = amount * timestep * playerControlComponent->linearSpeed;
				componentTransform->m_position += componentTransform->getForward() * -distance;
			}

			int strafeAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);

			if (strafeAxis > deadzone || strafeAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)strafeAxis) / 32767.0f;
				double distance = amount * timestep * playerControlComponent->linearSpeed;
				componentTransform->m_position += componentTransform->getRight() * distance;
			}

			static int lastButton = 0;

			int button = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
			if (button && !lastButton)
			{
				double rayDistance = 1000.0f;
				vector3D startPos = componentTransform->getPos();
				vector3D endPos = componentTransform->getPos() + (componentTransform->getForward() * rayDistance);
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
