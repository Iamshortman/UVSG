#ifndef PXRIGIDBODY_HPP
#define PXRIGIDBODY_HPP

#include "entityxInclude.hpp"
#include "physxInclude.hpp"
#include "Components.hpp"
#include "Util.hpp"

//Prototype Class for the world.
class PhysxWorld;

class RigidBodyPx
{
public:
	RigidBodyPx(PhysxWorld* world, Entity& entity, PxGeometry* shape, PxMaterial* material, float mass);

	physx::PxRigidDynamic* body;

private:
	PhysxWorld* phyxsWorld;

};

#endif //PXRIGIDBODY_HPP