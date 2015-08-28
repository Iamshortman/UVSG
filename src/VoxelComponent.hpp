#ifndef VOXELCOMPONENT_HPP
#define VOXELCOMPONENT_HPP

//#include <btBulletDynamicsCommon.h>

#include "Mesh.hpp"
#include "physxInclude.hpp"

typedef unsigned short BlockID;

class VoxelComponent
{
public:
	const static unsigned int chunkSize = 8;

	VoxelComponent();
	~VoxelComponent();

	void setBlock(int x, int y, int z, BlockID id);
	BlockID getBlock(int x, int y, int z);

	void setShouldUpdateChunk();
	void clearShouldUpdateChunk();
	bool getShouldUpdateChunk();

	float getCubeSize(){ return cubeSize; };

	physx::PxShape* collisionChunk[chunkSize][chunkSize][chunkSize];

	vector3 centerOfMassOffest;

	PxMaterial* material = nullptr;
	physx::PxBoxGeometry* cube = nullptr;
private:
	//Should Physics and Mesh be recalulated for this object?
	//Needs to be ralculated the first update.
	bool shouldUpdateChunk = true;

	//Stores all the block ideas for this object
	BlockID chunk[chunkSize][chunkSize][chunkSize];

	//The width of each cubes in the chunk.
	float cubeSize = 4.0f;
};


#endif //VOXELCOMPONENT_HPP