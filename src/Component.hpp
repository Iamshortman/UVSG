#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "GameObject.hpp"

class Component
{
    public:
        GameObject* parent;

        Component(GameObject* object);
        virtual ~Component();
        virtual void update() = 0;

    protected:
    private:
};

#endif // COMPONENT_HPP

