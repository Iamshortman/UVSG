#ifndef WORLD_HPP
#define WORLD_HPP

//Prototype Class
class World;

#include <set>
#include <algorithm>

#include "World/Entity.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/Lights/Lights.hpp"


//Prototype Classes
class Renderer;

class World
{
private:
	std::set<EntityId> m_entitiesInWorld;

public:
	World();
	virtual ~World();
	void updateWorld(double deltaTime);
	void render(Camera* camera);
	void renderFarView(Camera* camera);

	void addEntityToWorld(EntityId entityId);
	void removeEntityFromWorld(EntityId entityId);
	void getEntitiesInWorld(vector<Entity*> &entities);

	vector3D getGravity();
	void setGravity(vector3D gravity);

	virtual Transform getWorldOffsetMatrix();

	PhysicsWorld* m_physicsWorld; 

	Renderer* m_renderer;
	Renderer* m_PlanetRenderer;

	vector3F ambientLight = vector3F(0.4f);

};

#endif //WORLD_HPP