#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "TexturedMesh.hpp"

typedef unsigned short BlockID;

class Chunk
{

public:
	const static unsigned int chunkSize = 16;

	Chunk();
	~Chunk();

	void setBlock(int x, int y, int z, BlockID id);
	BlockID getBlock(int x, int y, int z);

	void updateChunk();
	void render();

	float getCubeSize(){ return cubeSize; };

private:

	//Should Physics and Mesh be recalulated for this object?
	//Needs to be ralculated the first update.
	bool shouldUpdateChunk = true;

	//Stores all the block ideas for this object
	BlockID blocks[chunkSize][chunkSize][chunkSize];

	//The width of each cubes in the chunk.
	float cubeSize = 1.0f;
	TexturedMesh* mesh = 0;
};

#endif CHUNK_HPP
