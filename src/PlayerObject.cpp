#include "PlayerObject.hpp"

PlayerObject::PlayerObject(World* worldPtr, unsigned int id)
:GameObject(worldPtr, id)
{

}

void PlayerObject::update()
{
    GameObject::update();
    btTransform temp;
    body->getMotionState()->getWorldTransform(temp);
    this->transform = temp;
    m_pGhostObject->setWorldTransform(this->transform);
}

void PlayerObject::render()
{
    GameObject::render();
}

PlayerObject::~PlayerObject()
{
    //dtor
}
