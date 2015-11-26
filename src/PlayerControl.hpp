#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include "entityxInclude.hpp"

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
				float amount = ((float)pitchAxis) / 32767.0f;
				float angle = amount * timestep * playerControlComponent->angularSpeed;

				//Negitive angle because the joystick layout is backwards
				quaternion pitchQuat = glm::normalize(glm::angleAxis( -angle, componentTransform->getRight() ));

				componentTransform->m_orientation = pitchQuat * componentTransform->m_orientation;
			}

			int yawAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

			if (yawAxis > deadzone || yawAxis < -deadzone)
			{
				//Get between -1 and 1
				float amount = ((float)yawAxis) / 32767.0f;
				float angle = amount * timestep * playerControlComponent->angularSpeed;

				quaternion yawQuat = glm::normalize(glm::angleAxis( -angle, vector3(0.0f, 1.0f, 0.0f) ));

				componentTransform->m_orientation = yawQuat * componentTransform->m_orientation;
			}


			int forwardAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY); //SDL_JoystickGetAxis(joystick, 1);

			if (forwardAxis > deadzone || forwardAxis < -deadzone)
			{
				//Get between -1 and 1
				float amount = ((float)forwardAxis) / 32767.0f;
				float distance = amount * timestep * playerControlComponent->linearSpeed;
				componentTransform->m_position += componentTransform->getForward() * -distance;
			}

			int strafeAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);

			if (strafeAxis > deadzone || strafeAxis < -deadzone)
			{
				//Get between -1 and 1
				float amount = ((float)strafeAxis) / 32767.0f;
				float distance = amount * timestep * playerControlComponent->linearSpeed;
				componentTransform->m_position += componentTransform->getRight() * distance;
			}

			static int lastButton = 0;

			int button = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
			if (button && !lastButton)
			{
				float rayDistance = 1000.0f;
				vector3 startPos = componentTransform->getPos();
				vector3 endPos = componentTransform->getPos() + (componentTransform->getForward() * rayDistance);
				SingleRayTestResults result = UVSG::getInstance()->physicsWorld->singleRayTest(startPos, endPos);

				if (result.hasHit)
				{

					Chunk* chunk = (Chunk*) result.hitBody->getUserPointer();
					if (chunk != nullptr)
					{
						printVec(result.hitPosition); printEndLine();
						printVec(result.hitNormal); printEndLine();

						Transform chunkTransform = chunk->getTransform();
						matrix4 invModelMatrix = glm::inverse(chunkTransform.getModleMatrix());
						vector4 localHitPos4 = invModelMatrix * vector4(result.hitPosition, 1.0f);
						vector3 localHitPos = vector3(localHitPos4);
						vector3 normalAdjust = result.hitNormal * -0.5f;

						localHitPos += normalAdjust;

						int x = localHitPos.x;
						int y = localHitPos.y;
						int z = localHitPos.z;

						chunk->setBlock(x, y, z, 0);

						std::printf("HitPos: {%i, %i, %i}\n", x, y, z);

					}

				}

			}
			lastButton = button;

		}
	};
};

#endif //PLAYERCONTROL_HPP