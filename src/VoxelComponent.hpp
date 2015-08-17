#ifndef VOXELCOMPONENT_HPP
#define VOXELCOMPONENT_HPP

#include "Mesh.hpp"
#include <btBulletDynamicsCommon.h>

typedef unsigned short BlockID;

class VoxelComponent
{
public:
	const static unsigned int chunkSize = 8;

	VoxelComponent()
	{
		for (unsigned int x = 0; x < chunkSize; x++)
		{
			for (unsigned int y = 0; y < chunkSize; y++)
			{
				for (unsigned int z = 0; z < chunkSize; z++)
				{
					collisionChunk[x][y][z] = 0;
				}
			}
		}
	};

	void setBlock(int x, int y, int z, BlockID id);
	BlockID getBlock(int x, int y, int z);

	void setShouldUpdateChunk();
	void clearShouldUpdateChunk();
	bool getShouldUpdateChunk();

	float getCubeSize(){ return cubeSize; };

	btCollisionShape* collisionChunk[chunkSize][chunkSize][chunkSize];

	vector3 centerOfMassOffest;
private:
	//Should Physics and Mesh be recalulated for this object?
	//Needs to be ralculated the first update.
	bool shouldUpdateChunk = true;

	//Stores all the block ideas for this object
	BlockID chunk[chunkSize][chunkSize][chunkSize];

	float cubeSize = 1.0f;
};


#endif //VOXELCOMPONENT_HPP