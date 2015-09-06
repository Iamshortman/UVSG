#ifndef THRUSTERCONTROL_HPP
#define THRUSTERCONTROL_HPP

#include "entityxInclude.hpp"
#include <SDL2/SDL.h>


struct ThrusterControlComponent
{
	//Mostly a placeholder
	bool inertiaDampening;

	//Acceleration in m/s per second;
	float linearAcceleration;

	//Max Speed in m/s
	float maxSpeed;
};

class  ThrusterControlSystem : public System < ThrusterControlSystem >
{
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override
	{
		//For all entities with the Thruster control and Velocity.
		ComponentHandle<ThrusterControlComponent> componentThrusterControlSearch;
		ComponentHandle<Velocity> componentVelocitySearch;
		ComponentHandle<Transform> componentTransformSerach;

		for (Entity entity : es.entities_with_components(componentThrusterControlSearch, componentVelocitySearch, componentTransformSerach))
		{
			//Get the component
			ComponentHandle<ThrusterControlComponent> componentThrusterControl = entity.component<ThrusterControlComponent>();
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();

			const Uint8 *state = SDL_GetKeyboardState(NULL);

			vector3 linearVelocityDelta(0.0f);

			if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_S])
			{
				float amount = state[SDL_SCANCODE_W] ? 1.0f : -1.0f;
				vector3 forward = componentTransform->getForward();
				forward *= (componentThrusterControl->linearAcceleration * dt);
				forward *= amount;

				linearVelocityDelta += forward;
			}

			if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D])
			{
				float amount = state[SDL_SCANCODE_D] ? 1.0f : -1.0f;
				vector3 right = componentTransform->getRight();
				right *= (componentThrusterControl->linearAcceleration * dt);
				right *= amount;

				linearVelocityDelta += right;
			}

			if (state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_LSHIFT])
			{
				float amount = state[SDL_SCANCODE_SPACE] ? 1.0f : -1.0f;
				vector3 up = componentTransform->getUp();
				up *= (componentThrusterControl->linearAcceleration * dt);
				up *= amount;

				linearVelocityDelta += up;
			}

			//Applies the changes in velocity
			componentVelocity->linearVelocity += linearVelocityDelta;
			
			if (state[SDL_SCANCODE_Z])
			{
				componentThrusterControl->inertiaDampening = false;
			}
			else
			{
				componentThrusterControl->inertiaDampening = true;
			}

			if (componentThrusterControl->inertiaDampening)
			{
				float maxAcceleration = (componentThrusterControl->linearAcceleration * dt);

				//Amount the object is moveing forward(+) or back(-).
				float forwardVelocity = glm::dot(componentTransform->getForward(), componentVelocity->linearVelocity);

				//Amount the object is moveing right(+) or left(-).
				float rightVelocity = glm::dot(componentTransform->getRight(), componentVelocity->linearVelocity);

				//Amount the object is moveing up(+) or down(-).
				float upVelocity = glm::dot(componentTransform->getUp(), componentVelocity->linearVelocity);

				if (!state[SDL_SCANCODE_W] && forwardVelocity > 0.0f)
				{
					vector3 slowDown = componentTransform->getForward() * maxAcceleration;
					componentVelocity->linearVelocity -= slowDown;

					//Recalulate the dot product
					forwardVelocity = glm::dot(componentTransform->getForward(), componentVelocity->linearVelocity);

					if (forwardVelocity < 0.0)
					{
						componentVelocity->linearVelocity += componentTransform->getForward() * -forwardVelocity;
					}
				}

				if (!state[SDL_SCANCODE_S] && forwardVelocity < 0.0f)
				{
					vector3 slowDown = componentTransform->getForward() * maxAcceleration;
					componentVelocity->linearVelocity += slowDown;

					//Recalulate the dot product
					forwardVelocity = glm::dot(componentTransform->getForward(), componentVelocity->linearVelocity);

					if (forwardVelocity > 0.0)
					{
						componentVelocity->linearVelocity += componentTransform->getForward() * -forwardVelocity;
					}
				}

				//Right Damping
				if (!state[SDL_SCANCODE_D] && rightVelocity > 0.0f)
				{
					vector3 slowDown = componentTransform->getRight() * maxAcceleration;
					componentVelocity->linearVelocity -= slowDown;

					//Recalulate the dot product
					rightVelocity = glm::dot(componentTransform->getRight(), componentVelocity->linearVelocity);

					if (rightVelocity < 0.0)
					{
						componentVelocity->linearVelocity += componentTransform->getRight() * -rightVelocity;
					}
				}

				//Left
				if (!state[SDL_SCANCODE_A] && rightVelocity < 0.0f)
				{
					vector3 slowDown = componentTransform->getRight() * maxAcceleration;
					componentVelocity->linearVelocity += slowDown;

					//Recalulate the dot product
					rightVelocity = glm::dot(componentTransform->getRight(), componentVelocity->linearVelocity);

					if (rightVelocity > 0.0)
					{
						componentVelocity->linearVelocity += componentTransform->getRight() * -rightVelocity;
					}
				}

				//Up Damping
				if (!state[SDL_SCANCODE_SPACE] && upVelocity > 0.0f)
				{
					vector3 slowDown = componentTransform->getUp() * maxAcceleration;
					componentVelocity->linearVelocity -= slowDown;

					//Recalulate the dot product
					upVelocity = glm::dot(componentTransform->getUp(), componentVelocity->linearVelocity);

					if (upVelocity < 0.0)
					{
						componentVelocity->linearVelocity += componentTransform->getUp() * -upVelocity;
					}
				}

				//Down
				if (!state[SDL_SCANCODE_LSHIFT] && upVelocity < 0.0f)
				{
					vector3 slowDown = componentTransform->getUp() * maxAcceleration;
					componentVelocity->linearVelocity += slowDown;

					//Recalulate the dot product
					upVelocity = glm::dot(componentTransform->getUp(), componentVelocity->linearVelocity);

					if (upVelocity > 0.0)
					{
						componentVelocity->linearVelocity += componentTransform->getUp() * -upVelocity;
					}
				}
			}

		}
	};
};


#endif //THRUSTERCONTROL_HPP