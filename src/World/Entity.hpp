#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <hash_map>
#include <string>
#include <vector>

#include "Physics/RigidBody.hpp"
#include "Components/Component.hpp"
#include "Components/Velocity.hpp"
#include "Components/Transform.hpp"

//Prototype Classes
class World;
class Renderer;

typedef uint32_t EntityId;

class Entity
{
private:
	World* m_world = nullptr;
	std::hash_map<std::string, Component*> m_components;
	bool m_alive = true;

	Transform m_transform;
	Velocity m_velocity;
	RigidBody* m_RigidBody = nullptr;

public:
	Renderer* m_renderer = nullptr;
	std::vector<void*> tempModels;

	const EntityId m_entityId;
	Entity(EntityId id);
	virtual ~Entity();

	bool isAlive() { return m_alive; };
	void kill();

	void update(double deltaTime);

	void addToWorld(World* world);
	World* getWorld();

	void addRigidBody(RigidBody* rigidBody);

	//Transform Functions
	vector3D getPosition();
	void setPosition(vector3D pos);

	quaternionD getOrientation();
	void setOrientation(quaternionD rot);

	vector3D getScale();
	void setScale(vector3D scale);

	Transform getTransform();
	void setTransform(Transform transform);

	//Velocity Functions
	vector3D getLinearVelocity() const;
	void setLinearVelocity(vector3D Velocity);

	vector3D getAngularVelocity() const;
	void setAngularVelocity(vector3D Velocity);

	void applyCentralForce(vector3D force);
	void applyCentralImpulse(vector3D impulse);

	void applyTorque(vector3D torque);
	void applyTorqueImpulse(vector3D torque);

	//Component functions 
	bool hasComponent(std::string componentName);
	Component* getComponent(std::string componentName);
	void addComponent(std::string componentName, Component* component);
	void removeComponent(std::string componentName);
};

#endif //ENTITY_HPP