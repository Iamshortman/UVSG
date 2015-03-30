#ifndef WORLD_HPP
#define WORLD_HPP

#include "PhysicsWorld.hpp"
#include "GameObject.hpp"

#include <vector>

class World
{
    public:
        World();
        virtual ~World();
        std::vector<GameObject>* gameObjects;

    protected:
    private:
        PhysicsWorld worldPhysics;

};

#endif // WORLD_HPP
