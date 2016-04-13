#ifndef WORLD_HPP
#define WORLD_HPP

//Prototype Class
class World;

#include <set>
#include <algorithm>
#include "World/Entity.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Rendering/Camera.hpp"

class World
{
private:
	std::set<EntityId> m_entitiesInWorld;

public:
	World();
	virtual ~World();
	void updateWorld(double deltaTime);
	void render(Camera* camera);

	void addEntityToWorld(EntityId entityId);
	void removeEntityFromWorld(EntityId entityId);
	void getEntitiesInWorld(vector<Entity*> &entities);

	vector3D getGravity();
	void setGravity(vector3D gravity);

	virtual Transform getWorldOffsetMatrix();

	PhysicsWorld* m_physicsWorld;

	void* m_renderer;
};

#endif //WORLD_HPP