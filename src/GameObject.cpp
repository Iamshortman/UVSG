#include "GameObject.hpp"
#include "World.hpp"
#include "Component.hpp"
#include <iostream>

GameObject::GameObject(World* worldPtr, unsigned int id) :
object_id(id)
{
	setWorldPtr(worldPtr);
    transform = btTransform();
    transform.setIdentity();
}

bool GameObject::isVoxel()
{
	return false;
}

void GameObject::addComponent(Component* component)
{
    components.push_back(component);
}

void GameObject::removeComponet(Component* componentToRemove)
{
    for(unsigned int i = 0; i < components.size(); i++)
    {
        if(components[i] == componentToRemove)
        {
            components.erase(components.begin() + i);
        }
    }
}

void GameObject::update()
{
    for(unsigned int i = 0; i < components.size(); i++)
    {
        components[i]->update();
    }
}

void GameObject::render()
{
    for(unsigned int i = 0; i < components.size(); i++)
    {
        components[i]->render();
    }
}

void GameObject::destoryObject()
{
    worldPtr->deleteGameObject(object_id);
}

GameObject::~GameObject()
{
    for(unsigned int i = 0; i < components.size(); i++)
    {
        delete components[i];
    }
}
