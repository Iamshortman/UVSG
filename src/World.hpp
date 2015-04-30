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

        unsigned int createGameObject(const btVector3& pos);
        unsigned int createPlayer(const  btVector3& pos);
        unsigned int createVoxelObject(const  btVector3& pos);
        unsigned int createCube(const btVector3& pos, const btVector3& size);
        int getGameObjectCount();

        GameObject* rayTrace(const btVector3& start, const btVector3& end);

    protected:
    private:

};

#endif // WORLD_HPP
