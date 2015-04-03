#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Component.hpp"

#include <btBulletDynamicsCommon.h>
#include <vector>

class GameObject
{
    public:
        GameObject(unsigned int id);
        ~GameObject();
        std::vector<Component> components;

        void addRigidBody(btRigidBody* rigidBody);

        const unsigned int object_id;
        btRigidBody* body;

};

#endif
