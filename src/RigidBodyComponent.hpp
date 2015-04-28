#ifndef RIGIDBODYCOMPONENT_HPP
#define RIGIDBODYCOMPONENT_HPP

#include "Component.hpp"

class RigidBodyComponent: public Component
{
    public:
        RigidBodyComponent(GameObject* object, btTransform& offsetTransform, btCollisionShape* shape, float mass);
        ~RigidBodyComponent();
        virtual void update();
        virtual inline void render(){}

        btRigidBody* rigidBody;
        btTransform offset;

    protected:
    private:
};

#endif // RIGIDBODYCOMPONENT_HPP
