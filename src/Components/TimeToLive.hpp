#ifndef TIMETOLIVE_HPP
#define TIMETOLIVE_HPP

#include "Components/Component.hpp"
#include "World/Entity.hpp"

struct TimeToLive: public Component
{
	//The Time left in seconds
	double timeToLive;

	TimeToLive(double lifeTime)
	{
		timeToLive = lifeTime;
	};

	virtual void update(double deltaTime) 
	{
		timeToLive -= deltaTime;
		if (timeToLive <= 0.0)
		{
			getParent()->kill();
		}
	};
};

#endif //TIMETOLIVE_HPP