#include <entityx/entityx.h>
#include <entityx\System.h>

#include "VoxelComponent.hpp"

#ifndef VOXELSYSTEM_HPP
#define VOXELSYSTEM_HPP

class VoxelSystem : public entityx::System < VoxelSystem >
{
	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		//For all entities with the time to live component.
		entityx::ComponentHandle<VoxelComponent> voxelComponentSearch;
		for (entityx::Entity entity : es.entities_with_components(voxelComponentSearch))
		{

		}
	};
};

#endif //VOXELSYSTEM_HPP