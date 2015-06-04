#ifndef PLAYEROBJECT_HPP
#define PLAYEROBJECT_HPP

#include "GameObject.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class PlayerObject: public GameObject
{
    public:
		PlayerObject(World* worldPtr, unsigned int id);
        virtual ~PlayerObject();
        virtual void render();
        virtual void update();

        btRigidBody* body;
        btPairCachingGhostObject* m_pGhostObject;
        void ParseGhostContacts();

    protected:
    private:
};

#endif // PLAYEROBJECT_HPP
