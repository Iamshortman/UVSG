#ifndef VOXELOBJECT_HPP
#define VOXELOBJECT_HPP

#include "GameObject.hpp"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <vector>

#include "glmInclude.hpp"
#include "Mesh.hpp"

class VoxelObject : public GameObject
{
    public:
        const static unsigned int chunkSize = 8;
        VoxelObject(unsigned int id);
        virtual ~VoxelObject();
        virtual void render();
        virtual void update();
        unsigned short chunk[chunkSize][chunkSize][chunkSize];
        Mesh voxelMesh;
        btRigidBody* body;
    protected:
    private:

        btCompoundShape* voxels;
};

#endif // VOXELOBJECT_HPP
