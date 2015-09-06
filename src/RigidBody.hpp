#ifndef PXRIGIDBODY_HPP
#define PXRIGIDBODY_HPP

#include "entityxInclude.hpp"
#include "physxInclude.hpp"
#include "Components.hpp"
#include "Util.hpp"

//Prototype Class for the world.
class PhysxWorld;

class RigidBody
{
public:
	RigidBody(PhysxWorld* world, Entity& entity, PxGeometry* shape, PxMaterial* material, float mass);

	physx::PxRigidDynamic* physicsBody;
	const PhysxWorld* getPhysxWorld(){ return phyxsWorld; };

private:
	PhysxWorld* phyxsWorld;

};

class StaticBody
{
public:
	StaticBody(PhysxWorld* world, Entity& entity, PxGeometry* shape, PxMaterial* material);

	physx::PxRigidStatic* physicsBody;
	const PhysxWorld* getPhysxWorld(){ return phyxsWorld; };

private:
	PhysxWorld* phyxsWorld;

};

#endif //PXRIGIDBODY_HPP