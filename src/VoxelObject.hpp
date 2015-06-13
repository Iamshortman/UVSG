#ifndef VOXELOBJECT_HPP
#define VOXELOBJECT_HPP

#include "GameObject.hpp"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <vector>

#include "glmInclude.hpp"
#include "Mesh.hpp"

typedef unsigned short BlockID;

class VoxelObject : public GameObject
{
    public:
        const static unsigned int chunkSize = 16;
		VoxelObject(World* worldPtr, unsigned int id, float size = 1.0f);
        virtual ~VoxelObject();
        virtual void render();
        virtual void update();
		void initPhysics();

		void setBlock(int x, int y, int z, BlockID id);
		BlockID getBlock(int x, int y, int z);
		void setUpdateChunk();
		void updateChunk();
		virtual bool isVoxel();
        const float getChunkScale();

        Mesh voxelMesh;
        btRigidBody* rigidBody;
    protected:
    private:
		bool shouldUpdateChunk = false;
		const float cubeSize;
		btCollisionShape* collisionChunk[chunkSize][chunkSize][chunkSize];
		BlockID chunk[chunkSize][chunkSize][chunkSize];
        btCompoundShape* voxels;
		btEmptyShape* emptyShape;
};

#endif // VOXELOBJECT_HPP
