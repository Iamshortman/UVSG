#include "PlayerObject.hpp"

PlayerObject::PlayerObject(unsigned int id)
:GameObject(id)
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
