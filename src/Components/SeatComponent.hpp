#ifndef SEATCOMPONENT_HPP
#define SEATCOMPONENT_HPP

#include "Components/Component.hpp"
#include "World/Entity.hpp"
#include "World/EntityManager.hpp"

struct SeatComponent : public Component
{
	Transform m_offest = Transform();
	Entity* m_occupyingEntity = nullptr;

	SeatComponent(Transform offest)
	{
		m_offest = offest;
	};

	virtual void update(double deltaTime)
	{
		Entity* parent = getParent();
		if (parent == nullptr)
		{
			printf("Error: No parent!!!\n");
			return;
		}

		if (m_occupyingEntity != nullptr)
		{
			Transform transform = parent->getTransform();

			transform.m_orientation *= m_offest.m_orientation;

			vector3D pos = transform.getOrientation() * m_offest.getPosition();
			transform.m_position += pos;

			m_occupyingEntity->setTransform(transform);
		}
	};

	void setOccupied(Entity* entity)
	{
		if (m_occupyingEntity == nullptr)
		{
			m_occupyingEntity = entity;
			m_occupyingEntity->ridingEntity = getParent();
		}
	};

	//TODO teleport entity to correct location
	void clearOccupancy()
	{
		if (m_occupyingEntity != nullptr)
		{ 
			m_occupyingEntity->ridingEntity = nullptr;
			m_occupyingEntity = nullptr;
		}
	};

	bool isOccupied()
	{
		return m_occupyingEntity != nullptr;
	}

};
#endif //SEATCOMPONENT_HPP