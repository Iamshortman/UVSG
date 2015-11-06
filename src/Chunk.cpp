#include "Chunk.hpp"
#include "glmInclude.hpp"
#include "Util.hpp"
#include <vector>
#include "UVSG.hpp"

Chunk::Chunk()
{
	for (unsigned int x = 0; x < chunkSize; x++)
	{
		for (unsigned int y = 0; y < chunkSize; y++)
		{
			for (unsigned int z = 0; z < chunkSize; z++)
			{
				blocks[x][y][z] = 1;
			}
		}
	}
}

void Chunk::setBlock(int x, int y, int z, BlockID id)
{
	if (x >= 0 && x < chunkSize)
	{
		if (y >= 0 && y < chunkSize)
		{
			if (z >= 0 && z < chunkSize)
			{
				blocks[x][y][z] = id;
				shouldUpdateChunk = true;
			}
		}
	}
}

BlockID Chunk::getBlock(int x, int y, int z)
{
	if (x >= 0 && x < chunkSize)
	{
		if (y >= 0 && y < chunkSize)
		{
			if (z >= 0 && z < chunkSize)
			{
				return blocks[x][y][z];
			}
		}
	}
	return 0;
}

Transform Chunk::getTransform()
{
	Transform transform;
	transform.setPos(vector3(0, -16.0f, 0));
	return transform;
}


void Chunk::updateChunk()
{
	if (rigidBody == nullptr)
	{
		rigidBody = new RigidBody(UVSG::getInstance()->physicsWorld, new btEmptyShape(), 0.0f);
		rigidBody->rigidBody->setCenterOfMassTransform(btTransform(btQuaternion(), toBtVec3(vector3(0.0f, -16.0f, 0.0f))));
		rigidBody->rigidBody->setUserPointer(this);
	}

	if (!shouldUpdateChunk)
	{
		return;
	}

	shouldUpdateChunk = false;

	if (mesh != nullptr)
	{
		delete mesh;
		mesh = nullptr;
	}

	vector3 vertsCube[] =
	{
		vector3(0, 0, 0),
		vector3(0, 0, 1),
		vector3(0, 1, 0),
		vector3(0, 1, 1),
		vector3(1, 0, 0),
		vector3(1, 0, 1),
		vector3(1, 1, 0),
		vector3(1, 1, 1),
	};
	vector3 normals[] = 
	{
		vector3(0, 1, 0),
		vector3(0, -1, 0),
		vector3(0, 0, 1),
		vector3(0, 0, -1),
		vector3(1, 0, 0),
		vector3(-1, 0, 0),
	};
	vector2 textureUv[] = 
	{
		vector2(0.0f, 0.0f),
		vector2(0.0f, 1.0f),
		vector2(1.0f, 1.0f),
		vector2(1.0f, 0.0f)
	};

	std::vector<Vertex> verticesVector = std::vector<Vertex>();
	std::vector<unsigned int> indicesVector;
	unsigned int indicesOffset = 0;

	unsigned int blockCount = 0;

	for (unsigned int x = 0; x < chunkSize; x++)
	{
		for (unsigned int y = 0; y < chunkSize; y++)
		{
			for (unsigned int z = 0; z < chunkSize; z++)
			{
				if (blocks[x][y][z] != 0)
				{
					blockCount++;
					vector3 offset = vector3(x, y, z) * cubeSize;
					
					//Top
					if (getBlock(x, y + 1, z) == 0)
					{
						verticesVector.push_back({ vertsCube[3] + offset, normals[0], textureUv[0] });
						verticesVector.push_back({ vertsCube[7] + offset, normals[0], textureUv[3] });
						verticesVector.push_back({ vertsCube[6] + offset, normals[0], textureUv[2] });
						verticesVector.push_back({ vertsCube[2] + offset, normals[0], textureUv[1] });

						//Clockwise ordering
						indicesVector.push_back(0 + indicesOffset);
						indicesVector.push_back(1 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(3 + indicesOffset);
						indicesVector.push_back(0 + indicesOffset);
						indicesOffset += 4;
					}

					//Bottom
					if (getBlock(x, y - 1, z) == 0)
					{
						verticesVector.push_back({ vertsCube[5] + offset, normals[1], textureUv[0] });
						verticesVector.push_back({ vertsCube[1] + offset, normals[1], textureUv[3] });
						verticesVector.push_back({ vertsCube[0] + offset, normals[1], textureUv[2] });
						verticesVector.push_back({ vertsCube[4] + offset, normals[1], textureUv[1] });

						//Clockwise ordering
						indicesVector.push_back(0 + indicesOffset);
						indicesVector.push_back(1 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(3 + indicesOffset);
						indicesVector.push_back(0 + indicesOffset);
						indicesOffset += 4;
					}

					//forward
					if (getBlock(x, y, z + 1) == 0)
					{
						verticesVector.push_back({ vertsCube[7] + offset, normals[2], textureUv[0] });
						verticesVector.push_back({ vertsCube[3] + offset, normals[2], textureUv[3] });
						verticesVector.push_back({ vertsCube[1] + offset, normals[2], textureUv[2] });
						verticesVector.push_back({ vertsCube[5] + offset, normals[2], textureUv[1] });

						//Clockwise ordering
						indicesVector.push_back(0 + indicesOffset);
						indicesVector.push_back(1 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(3 + indicesOffset);
						indicesVector.push_back(0 + indicesOffset);
						indicesOffset += 4;
					}

					//Back
					if (getBlock(x, y, z - 1) == 0)
					{
						verticesVector.push_back({ vertsCube[2] + offset, normals[3], textureUv[0] });
						verticesVector.push_back({ vertsCube[6] + offset, normals[3], textureUv[3] });
						verticesVector.push_back({ vertsCube[4] + offset, normals[3], textureUv[2] });
						verticesVector.push_back({ vertsCube[0] + offset, normals[3], textureUv[1] });

						//Clockwise ordering
						indicesVector.push_back(0 + indicesOffset);
						indicesVector.push_back(1 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(3 + indicesOffset);
						indicesVector.push_back(0 + indicesOffset);
						indicesOffset += 4;
					}

					//Left
					if (getBlock(x + 1, y, z) == 0)
					{
						verticesVector.push_back({ vertsCube[6] + offset, normals[4], textureUv[0] });
						verticesVector.push_back({ vertsCube[7] + offset, normals[4], textureUv[3] });
						verticesVector.push_back({ vertsCube[5] + offset, normals[4], textureUv[2] });
						verticesVector.push_back({ vertsCube[4] + offset, normals[4], textureUv[1] });

						//Clockwise ordering
						indicesVector.push_back(0 + indicesOffset);
						indicesVector.push_back(1 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(3 + indicesOffset);
						indicesVector.push_back(0 + indicesOffset);
						indicesOffset += 4;
					}

					//Right
					if (getBlock(x - 1, y, z) == 0)
					{
						verticesVector.push_back({ vertsCube[3] + offset, normals[5], textureUv[0] });
						verticesVector.push_back({ vertsCube[2] + offset, normals[5], textureUv[3] });
						verticesVector.push_back({ vertsCube[0] + offset, normals[5], textureUv[2] });
						verticesVector.push_back({ vertsCube[1] + offset, normals[5], textureUv[1] });

						//Clockwise ordering
						indicesVector.push_back(0 + indicesOffset);
						indicesVector.push_back(1 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(2 + indicesOffset);
						indicesVector.push_back(3 + indicesOffset);
						indicesVector.push_back(0 + indicesOffset);
						indicesOffset += 4;
					}

				}
			}
		}
	}

	this->mesh = new TexturedMesh(verticesVector, indicesVector);

	btTriangleMesh *mesh = new btTriangleMesh();
	for (int i = 0; i < indicesVector.size(); i += 3)
	{
		mesh->addTriangle(
			toBtVec3(verticesVector[ indicesVector[i] ].pos),
			toBtVec3(verticesVector[ indicesVector[i + 1] ].pos),
			toBtVec3(verticesVector[ indicesVector[i + 2] ].pos)
			);
	}
	
	rigidBody->setCollisionShape(true, new btBvhTriangleMeshShape(mesh, true, true));
}

void Chunk::render()
{
	if (mesh != 0)
	{
		mesh->draw();
	}
}

Chunk::~Chunk()
{

}