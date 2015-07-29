#include <entityx/entityx.h>
#include <entityx\System.h>

#ifndef TIMETOLIVE_HPP
#define TIMETOLIVE_HPP

struct TimeToLiveComponent
{
	TimeToLiveComponent(entityx::TimeDelta lifeTime)
	{
		timeToLive = lifeTime;
	};
	//The Time left in seconds
	entityx::TimeDelta timeToLive;
};

class TimeToLiveSystem : public entityx::System < TimeToLiveSystem >
{
	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		//For all entities with the time to live component.
		entityx::ComponentHandle<TimeToLiveComponent> timeToLiveComponentSearch;
		for (entityx::Entity entity : es.entities_with_components(timeToLiveComponentSearch))
		{
			//Get the component
			entityx::ComponentHandle<TimeToLiveComponent> timeToLiveComponent = entity.component<TimeToLiveComponent>();

			//Subtract the time from the previous tick
			timeToLiveComponent->timeToLive -= dt;

			//If the Entity is out of time, delete it
			if (timeToLiveComponent->timeToLive <= 0.0)
			{
				entity.destroy();
			}
		}
	};
};


#endif //TIMETOLIVE_HPP