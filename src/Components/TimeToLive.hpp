#ifndef TIMETOLIVE_HPP
#define TIMETOLIVE_HPP

#include "Components/Component.hpp"
#include "World/Entity.hpp"
#include "World/EntityManager.hpp"

struct TimeToLive: public Component
{
	//The Time left in seconds
	double m_timeToLive;

	TimeToLive(double lifeTime)
	{
		m_timeToLive = lifeTime;
	};

	virtual void update(double deltaTime) 
	{
		m_timeToLive -= deltaTime;
		if (m_timeToLive <= 0.0)
		{
			getParent()->kill();
		}
	};
};
#endif //TIMETOLIVE_HPP