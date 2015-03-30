#include "World.hpp"

World::World()
{
    gameObjects = new std::vector<GameObject>();
    worldPhysics = PhysicsWorld();
}

World::~World()
{
    delete gameObjects;
}
