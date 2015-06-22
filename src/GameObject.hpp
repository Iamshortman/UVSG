#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <btBulletDynamicsCommon.h>
#include <vector>
#include <iostream>
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

		GameObject(World* worldPtr, unsigned int id);
        virtual ~GameObject();
        void addComponent(Component* component);
        void removeComponet(Component* componentToRemove);
        virtual void update();
        void destoryObject();
		virtual bool isVoxel();

        virtual void render();

        inline void setWorldPtr(World* ptr){worldPtr = ptr;}
        inline const World* getWorldPtr(){return worldPtr;}
};

#endif
