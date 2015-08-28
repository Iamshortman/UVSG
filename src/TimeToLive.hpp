#ifndef TIMETOLIVE_HPP
#define TIMETOLIVE_HPP

#include <entityxInclude.hpp>

struct TimeToLiveComponent
{
	TimeToLiveComponent(TimeDelta lifeTime)
	{
		timeToLive = lifeTime;
	};
	//The Time left in seconds
	TimeDelta timeToLive;
};

class TimeToLiveSystem : public System < TimeToLiveSystem >
{
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override
	{
		//For all entities with the time to live component.
		ComponentHandle<TimeToLiveComponent> timeToLiveComponentSearch;
		for (Entity entity : es.entities_with_components(timeToLiveComponentSearch))
		{
			//Get the component
			ComponentHandle<TimeToLiveComponent> timeToLiveComponent = entity.component<TimeToLiveComponent>();

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