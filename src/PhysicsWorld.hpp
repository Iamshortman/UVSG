#include <btBulletDynamicsCommon.h>
#include <iostream>

#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

class PhysicsWorld
{
    public:
        PhysicsWorld();
        virtual ~PhysicsWorld();
        btRigidBody* groundRigidBody;
        btRigidBody* boxRigidBody;
        void updateWorld();
        bool rayTest(btVector3 start, btVector3 end);

    private:
        btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;

};

#endif // PHYSICSWORLD_HPP
