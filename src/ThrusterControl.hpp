#ifndef THRUSTERCONTROL_HPP
#define THRUSTERCONTROL_HPP

#include <entityxInclude.hpp>
#include <SDL2/SDL.h>


struct ThrusterControlComponent
{
	//Mostly a placeholder
	bool inertiaDampening;

	//Acceleration in m/s per second;
	float acceleration;

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

			if (state[SDL_SCANCODE_W])
			{
				vector3 forward = componentTransform->getForward();
				forward *= (componentThrusterControl->acceleration * dt);

				linearVelocityDelta += forward;
			}
			
			componentVelocity->linearVelocity += linearVelocityDelta;
		}
	};
};


#endif //THRUSTERCONTROL_HPP