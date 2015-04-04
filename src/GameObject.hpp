#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <btBulletDynamicsCommon.h>
#include <vector>
#include "World.hpp"

//Prototype class
class Component;

class GameObject
{
    public:
        const unsigned int object_id;
        World* worldPtr;
        std::vector<Component*> components;
        btTransform transform;

        GameObject(unsigned int id);
        ~GameObject();
        void addComponent(Component* component);
        void removeComponet(Component* componentToRemove);
        void update();
        void destoryObject();

        inline void setWorldPtr(World* ptr){worldPtr = ptr;}
        inline const World* getWorldPtr(){return worldPtr;}
};

#endif
