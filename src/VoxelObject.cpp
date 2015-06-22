#include "VoxelObject.hpp"
#include "World.hpp"
#include "PhysicsWorld.hpp"
#include "RigidBodyComponent.hpp"

#include <iostream>


VoxelObject::VoxelObject(World* worldPtr, unsigned int id, float size) :
GameObject(worldPtr, id),
cubeSize(size)
{
	//cube = new btBoxShape(btVector3(cubeSize / 2.0f, cubeSize / 2.0f, cubeSize / 2.0f));

	initPhysics();
	for (unsigned int x = 0; x < chunkSize; x++)
	{
		for (unsigned int y = 0; y < chunkSize; y++)
		{
			for (unsigned int z = 0; z < chunkSize; z++)
			{
				collisionChunk[x][y][z] = 0;
				setBlock(x, y, z, 1);

				/*if ((x + y + z) % 2 == 0)
				{
					setBlock(x, y, z, 1);
				}*/
			}
		}
	}

	rigidBody->forceActivationState(DISABLE_DEACTIVATION);
}

void VoxelObject::initPhysics()
{
	btScalar mass = 0.1f;
	btVector3 Inertia;
	voxels = new btCompoundShape();
	voxels->calculateLocalInertia(mass, Inertia);
	btDefaultMotionState* MotionState = new btDefaultMotionState(this->transform);
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, MotionState, voxels, Inertia);
	rigidBody = new btRigidBody(boxRigidBodyCI);
	rigidBody->setUserPointer(this);
	worldPtr->worldPhysics->addRigidBody(rigidBody);

	//Add an empty shape so the chunk doesnt crash even if it has no collision shapes.
	emptyShape = new btEmptyShape();
	voxels->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)), emptyShape);
}

void VoxelObject::setTransform(btTransform transformToSet)
{
    this->transform = transformToSet;
    this->rigidBody->getMotionState()->setWorldTransform(this->transform);
}


void VoxelObject::update()
{
    GameObject::update();
    btTransform temp;
    rigidBody->getMotionState()->getWorldTransform(temp);
    this->transform = temp;

	if (shouldUpdateChunk)
	{
		updateChunk();
		this->shouldUpdateChunk = false;
	}
}

void VoxelObject::render()
{
    GameObject::render();
    voxelMesh.draw();
}

void VoxelObject::setUpdateChunk()
{
	shouldUpdateChunk = true;
}

void VoxelObject::setBlock(int x, int y, int z, BlockID id)
{
	if (x >= 0 && x < chunkSize)
	{
		if (y >= 0 && y < chunkSize)
		{
			if (z >= 0 && z < chunkSize)
			{
				chunk[x][y][z] = id;
				shouldUpdateChunk = true;
			}
		}
	}
}

BlockID VoxelObject::getBlock(int x, int y, int z)
{
	if (x >= 0 && x < chunkSize)
	{
		if (y >= 0 && y < chunkSize)
		{
			if (z >= 0 && z < chunkSize)
			{
				return chunk[x][y][z];
			}
		}
	}
	return 0;
}

void VoxelObject::updateChunk()
{
	vector3 vertsCube[] =
	{
		vector3(0.5f, -0.5f, -0.5f) * cubeSize,
		vector3(0.5f, -0.5f, 0.5f) * cubeSize,
		vector3(-0.5f, -0.5f, 0.5f) * cubeSize,
		vector3(-0.5f, -0.5f, -0.5f) * cubeSize,
		vector3(0.5f, 0.5f, -0.5f) * cubeSize,
		vector3(0.5f, 0.5f, 0.5f) * cubeSize,
		vector3(-0.5f, 0.5f, 0.5f) * cubeSize,
		vector3(-0.5f, 0.5f, -0.5f) * cubeSize
	};

	vector3 colorsCube[] =
	{
		vector3(0.0F, 0.0F, 1.0F),
		vector3(0.0F, 1.0F, 0.0F),
		vector3(0.0F, 1.0F, 1.0F),
		vector3(1.0F, 0.0F, 0.0F),
		vector3(1.0F, 0.0F, 1.0F),
		vector3(1.0F, 1.0F, 0.0F),
		vector3(1.0F, 1.0F, 1.0F),
		vector3(0.0F, 0.0F, 0.0F)
	};


	std::vector<vector3> tempVertices = std::vector<vector3>();
	std::vector<vector3> tempColors = std::vector<vector3>();
	std::vector<unsigned int> tempIndices = std::vector<unsigned int>();
	unsigned int indicesOffset = 0;

	btScalar tempMass = 0.1f; //Starts out a little above 0 so its not static.
	vector3	tempPos(0, 0, 0);
    btVector3 centerOfMass(0, 0, 0);
    unsigned int blockCount = 0;

	for (unsigned int x = 0; x < chunkSize; x++)
	{
		for (unsigned int y = 0; y < chunkSize; y++)
		{
			for (unsigned int z = 0; z < chunkSize; z++)
			{
				tempPos = vector3(x, y, z);
				tempPos *= cubeSize;

				BlockID id = getBlock(x, y, z);
				if (id == 0)
				{
					if (collisionChunk[x][y][z] != 0)
					{
						voxels->removeChildShape(collisionChunk[x][y][z]);
						delete collisionChunk[x][y][z];
						collisionChunk[x][y][z] = 0;
					}
				}

                //TODO other IDS besides 1
				if (id == 1)
				{
                    btScalar blockMass = 1.0f;
                    blockCount++;
					tempMass += blockMass;
                    centerOfMass += (btVector3(x, y, z) * blockMass);

					//If surrounded by blocks do nothing
					if (getBlock(x + 1, y, z) == 1 && getBlock(x - 1, y, z) == 1 && getBlock(x, y + 1, z) == 1 && getBlock(x, y - 1, z) == 1 && getBlock(x, y, z + 1) == 1 && getBlock(x, y, z - 1) == 1)
					{
						if (collisionChunk[x][y][z] != 0)
						{
							voxels->removeChildShape(collisionChunk[x][y][z]);
							delete collisionChunk[x][y][z];
							collisionChunk[x][y][z] = 0;
						}

						continue;
					}

					if (collisionChunk[x][y][z] == 0)
					{
						collisionChunk[x][y][z] = new btBoxShape(btVector3(cubeSize / 2.0f, cubeSize / 2.0f, cubeSize / 2.0f));
						voxels->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), btVector3(x, y, z) * cubeSize), collisionChunk[x][y][z]);
					}

					//Top
					if (getBlock(x, y + 1, z) == 0)
					{
						tempIndices.push_back(0 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempIndices.push_back(1 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempIndices.push_back(2 + indicesOffset);
						tempIndices.push_back(1 + indicesOffset);
						tempVertices.push_back(vertsCube[4] + tempPos);
						tempVertices.push_back(vertsCube[5] + tempPos);
						tempVertices.push_back(vertsCube[6] + tempPos);
						tempVertices.push_back(vertsCube[7] + tempPos);
						tempColors.push_back(colorsCube[4]);
						tempColors.push_back(colorsCube[5]);
						tempColors.push_back(colorsCube[6]);
						tempColors.push_back(colorsCube[7]);
						indicesOffset += 4;
					}

					//Bottom
					if (getBlock(x, y - 1, z) == 0)
					{
						tempIndices.push_back(0 + indicesOffset);
						tempIndices.push_back(1 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempIndices.push_back(1 + indicesOffset);
						tempIndices.push_back(2 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempVertices.push_back(vertsCube[0] + tempPos);
						tempVertices.push_back(vertsCube[1] + tempPos);
						tempVertices.push_back(vertsCube[2] + tempPos);
						tempVertices.push_back(vertsCube[3] + tempPos);
						tempColors.push_back(colorsCube[0]);
						tempColors.push_back(colorsCube[1]);
						tempColors.push_back(colorsCube[2]);
						tempColors.push_back(colorsCube[3]);
						indicesOffset += 4;
					}

					//Forward
					if (getBlock(x, y, z + 1) == 0)
					{
						tempIndices.push_back(0 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempIndices.push_back(1 + indicesOffset);
						tempIndices.push_back(0 + indicesOffset);
						tempIndices.push_back(2 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempVertices.push_back(vertsCube[1] + tempPos);
						tempVertices.push_back(vertsCube[2] + tempPos);
						tempVertices.push_back(vertsCube[5] + tempPos);
						tempVertices.push_back(vertsCube[6] + tempPos);
						tempColors.push_back(colorsCube[1]);
						tempColors.push_back(colorsCube[2]);
						tempColors.push_back(colorsCube[5]);
						tempColors.push_back(colorsCube[6]);
						indicesOffset += 4;
					}

					//Back
					if (getBlock(x, y, z - 1) == 0)
					{
						tempIndices.push_back(1 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempIndices.push_back(0 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempIndices.push_back(2 + indicesOffset);
						tempIndices.push_back(0 + indicesOffset);
						tempVertices.push_back(vertsCube[0] + tempPos);
						tempVertices.push_back(vertsCube[3] + tempPos);
						tempVertices.push_back(vertsCube[4] + tempPos);
						tempVertices.push_back(vertsCube[7] + tempPos);
						tempColors.push_back(colorsCube[0]);
						tempColors.push_back(colorsCube[3]);
						tempColors.push_back(colorsCube[4]);
						tempColors.push_back(colorsCube[7]);
						indicesOffset += 4;
					}

					//Left
					if (getBlock(x + 1, y, z) == 0)
					{
						tempIndices.push_back(0 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempIndices.push_back(1 + indicesOffset);
						tempIndices.push_back(0 + indicesOffset);
						tempIndices.push_back(2 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempVertices.push_back(vertsCube[0] + tempPos);
						tempVertices.push_back(vertsCube[1] + tempPos);
						tempVertices.push_back(vertsCube[4] + tempPos);
						tempVertices.push_back(vertsCube[5] + tempPos);
						tempColors.push_back(colorsCube[0]);
						tempColors.push_back(colorsCube[1]);
						tempColors.push_back(colorsCube[4]);
						tempColors.push_back(colorsCube[5]);
						indicesOffset += 4;
					}

					//Right
					if (getBlock(x - 1, y, z) == 0)
					{
						tempIndices.push_back(0 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempIndices.push_back(1 + indicesOffset);
						tempIndices.push_back(0 + indicesOffset);
						tempIndices.push_back(2 + indicesOffset);
						tempIndices.push_back(3 + indicesOffset);
						tempVertices.push_back(vertsCube[2] + tempPos);
						tempVertices.push_back(vertsCube[3] + tempPos);
						tempVertices.push_back(vertsCube[6] + tempPos);
						tempVertices.push_back(vertsCube[7] + tempPos);
						tempColors.push_back(colorsCube[2]);
						tempColors.push_back(colorsCube[3]);
						tempColors.push_back(colorsCube[6]);
						tempColors.push_back(colorsCube[7]);
						indicesOffset += 4;
					}
				}
			}
		}
	}
	centerOfMass /= blockCount;
    centerOfMassOffset = centerOfMass;

	//rigidBody->setCenterOfMassTransform(btTransform(btQuaternion(0, 0, 0, 1), centerOfMass));
	for (unsigned int x = 0; x < chunkSize; x++)
	{
		for (unsigned int y = 0; y < chunkSize; y++)
		{
			for (unsigned int z = 0; z < chunkSize; z++)
			{
                if(collisionChunk[x][y][z] != 0)
                {
                    voxels->
                }
                btTransform transform = voxels->getChildTransform(i);
                btVector3 pos = transform.getOrigin();
                pos -= centerOfMass;
                transform.setOrigin(pos);
                voxels->updateChildTransform(i, transform, false);
            }
        }
    }

    //std::cout << "{ " << centerOfMass.getX() << ", " << centerOfMass.getY() << ", " << centerOfMass.getZ() << " }\n";

	btVector3 inertia;
	rigidBody->getCollisionShape()->calculateLocalInertia(tempMass, inertia);
	rigidBody->setMassProps(tempMass, inertia);

	this->voxelMesh.addVertices(tempVertices, tempColors, tempIndices);

	//If we have no blocks delete the object.
	if(blockCount == 0)
	{
        this->worldPtr->deleteGameObject(this->object_id);
	}
}

const float VoxelObject::getChunkScale()
{
    return this->cubeSize;
}

VoxelObject::~VoxelObject()
{
    voxels->removeChildShape(this->emptyShape);
    delete this->emptyShape;
	for (unsigned int x = 0; x < chunkSize; x++)
	{
		for (unsigned int y = 0; y < chunkSize; y++)
		{
			for (unsigned int z = 0; z < chunkSize; z++)
			{
                voxels->removeChildShape(collisionChunk[x][y][z]);
                delete collisionChunk[x][y][z];
                collisionChunk[x][y][z] = 0;
            }
        }
    }
    this->worldPtr->worldPhysics->removeRigidBody(rigidBody);
    delete rigidBody->getMotionState();
    delete rigidBody->getCollisionShape();
    delete rigidBody;
}

bool VoxelObject::isVoxel()
{
	return true;
}
