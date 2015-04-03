#include "GameObject.hpp"
#include <iostream>

GameObject::GameObject(unsigned int id):
object_id(id)
{

}

void GameObject::addRigidBody(btRigidBody* rigidBody)
{
    body = rigidBody;
    body->setUserPointer(this);
}

GameObject::~GameObject()
{
    delete body->getMotionState();
    delete body->getCollisionShape();
    delete body;
}
