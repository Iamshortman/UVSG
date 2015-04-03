#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <iostream>

typedef btCollisionWorld::ClosestRayResultCallback ClosestRayResultCallback;

class PhysicsWorld
{
    public:
        PhysicsWorld();
        virtual ~PhysicsWorld();
        btRigidBody* groundRigidBody;
        void updateWorld(float timeStep);
        void addRigidBody(btRigidBody* body);
        void removeRigidBody(btRigidBody* body);

        ClosestRayResultCallback rayTest(btVector3 start, btVector3 end);

    private:
        btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;

};

#endif // PHYSICSWORLD_HPP
