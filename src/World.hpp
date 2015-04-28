#ifndef WORLD_HPP
#define WORLD_HPP

class World;

#include "PhysicsWorld.hpp"
//#include "GameObject.hpp"

#include <vector>

class GameObject;

class World
{
    public:
        World();
        virtual ~World();

        std::vector<GameObject*> gameObjects;
        PhysicsWorld* worldPhysics;

        void update(unsigned long delta);
        void addGameObject(GameObject object);
        GameObject* getGameObject(unsigned int object_id);
        void deleteGameObject(unsigned int object_id);

        unsigned int createGameObject(btVector3& pos);
        unsigned int createPlayer(btVector3& pos);
        unsigned int createVoxelObject(btVector3& pos);
        unsigned int createCube(btVector3& pos, btVector3& size);
        int getGameObjectCount();

        GameObject* rayTrace(btVector3& start, btVector3& end);

    protected:
    private:

};

#endif // WORLD_HPP
