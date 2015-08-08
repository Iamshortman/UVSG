#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include <entityx/entityx.h>
#include <entityx\System.h>

#include "UVSG.hpp"

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

class PlayerControlSystem : public entityx::System < PlayerControlSystem >
{
	quaternion fromAxes(vector3 forward, vector3 up)
	{
		vector3 right = glm::normalize(glm::cross(forward, up));

		matrix3 rotationMatrix = matrix3();

		rotationMatrix[0][0] = right.x;
		rotationMatrix[0][1] = right.y;
		rotationMatrix[0][2] = right.z;

		rotationMatrix[1][0] = up.x;
		rotationMatrix[1][1] = up.y;
		rotationMatrix[1][2] = up.z;

		rotationMatrix[2][0] = forward.x;
		rotationMatrix[2][1] = forward.y;
		rotationMatrix[2][2] = forward.z;

		return toQuat(rotationMatrix);
	};

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		float timestep = ((float)dt);
		SDL_GameController *controller = UVSG::getInstance()->controller;

		//For all entities with the Player Control and Tranform components.
		entityx::ComponentHandle<PlayerControlComponent> playerControlComponentSearch;
		entityx::ComponentHandle<Transform> transformComponentSearch;
		for (entityx::Entity entity : es.entities_with_components(playerControlComponentSearch, transformComponentSearch))
		{
			//Get the component
			entityx::ComponentHandle<PlayerControlComponent> playerControlComponent = entity.component<PlayerControlComponent>();
			entityx::ComponentHandle<Transform> componentTransform = entity.component<Transform>();
		
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