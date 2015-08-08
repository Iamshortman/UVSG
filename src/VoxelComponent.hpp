#ifndef VOXELCOMPONENT_HPP
#define VOXELCOMPONENT_HPP

#include "Mesh.hpp"
#include <btBulletDynamicsCommon.h>
#include <entityx\entityx.h>

typedef unsigned short BlockID;

class VoxelComponent
{
public:
	const static unsigned int chunkSize = 16;

	void setBlock(int x, int y, int z, BlockID id);
	BlockID getBlock(int x, int y, int z);

	void setShouldUpdateChunk();
	void clearShouldUpdateChunk();
	bool getShouldUpdateChunk();

	void draw();

	float getCubeSize(){ return cubeSize; };

	Mesh voxelMesh;
private:
	//Should Physics and Mesh be recalulated for this object?
	//Needs to be ralculated the first update.
	bool shouldUpdateChunk = true;

	//Stores all the block ideas for this object
	BlockID chunk[chunkSize][chunkSize][chunkSize];

	float cubeSize = 1.0f;
};


#endif //VOXELCOMPONENT_HPP