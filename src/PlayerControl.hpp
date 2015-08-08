#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include <entityx/entityx.h>
#include <entityx\System.h>
#include <entityxInclude.hpp>

#include "Util.hpp"

struct PlayerControlComponent
{
	PlayerControlComponent(float linear, float angular)
	{
		linearSpeed = linear;
		angularSpeed = angular;
	};

	float linearSpeed;

	//rad per second
	float angularSpeed;
};

class PlayerControlSystem : public System < PlayerControlSystem >
{
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override
	{
		float timestep = ((float)dt);
		SDL_GameController *controller = UVSG::getInstance()->controller;

		//For all entities with the Player Control and Tranform components.
		ComponentHandle<PlayerControlComponent> playerControlComponentSearch;
		ComponentHandle<Transform> transformComponentSearch;
		for (Entity entity : es.entities_with_components(playerControlComponentSearch, transformComponentSearch))
		{
			//Get the component
			ComponentHandle<PlayerControlComponent> playerControlComponent = entity.component<PlayerControlComponent>();
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
		
			int deadzone = 8000;

			int pitchAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

			if (pitchAxis > deadzone || pitchAxis < -deadzone)
			{
				//Get between -1 and 1
				float amount = ((float)pitchAxis) / 32767.0f;
				float angle = amount * timestep * playerControlComponent->angularSpeed;

				//Negitive angle because the stick is backwards
				quaternion pitchQuat = glm::normalize(glm::angleAxis( -angle, componentTransform->getRight() ));

				componentTransform->orientation = pitchQuat * componentTransform->orientation;
			}

			int yawAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

			if (yawAxis > deadzone || yawAxis < -deadzone)
			{
				//Get between -1 and 1
				float amount = ((float)yawAxis) / 32767.0f;
				float angle = amount * timestep * playerControlComponent->angularSpeed;

				quaternion yawQuat = glm::normalize(glm::angleAxis( -angle, vector3(0.0f, 1.0f, 0.0f) ));

				componentTransform->orientation = yawQuat * componentTransform->orientation;
			}


			int movementY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

			if (movementY > deadzone || movementY < -deadzone)
			{
				//Get between -1 and 1
				float amount = ((float)movementY) / 32767.0f;
				float distance = amount * timestep * playerControlComponent->linearSpeed;

				componentTransform->position += componentTransform->getForward() * -distance;
			}

			int movementX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);

			if (movementX > deadzone || movementX < -deadzone)
			{
				//Get between -1 and 1
				float amount = ((float)movementX) / 32767.0f;
				float distance = amount * timestep * playerControlComponent->linearSpeed;

				componentTransform->position += componentTransform->getRight() * distance;
			}

		}
	};
};

#endif //PLAYERCONTROL_HPP