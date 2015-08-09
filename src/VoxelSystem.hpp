#ifndef VOXELSYSTEM_HPP
#define VOXELSYSTEM_HPP

#include "entityxInclude.hpp"
#include "VoxelComponent.hpp"

class VoxelSystem : public System < VoxelSystem >
{

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override
	{
		//For all entities with the Voxel component.
		ComponentHandle<VoxelComponent> componentVoxelSearch;
		for (Entity entity : es.entities_with_components(componentVoxelSearch))
		{
			ComponentHandle<VoxelComponent> componentVoxel = entity.component<VoxelComponent>();

			if (componentVoxel->getShouldUpdateChunk())
			{
				int result = rebuildChunk(entity, componentVoxel);

				//If result = -1, there are no blocks in the chunk, so delte it
				if (result == -1)
				{
					entity.destroy();
				}
			}
		}
	};


	int rebuildChunk(Entity entity, ComponentHandle<VoxelComponent> &componentVoxel)
	{
		//Reset flag
		componentVoxel->clearShouldUpdateChunk();

		float cubeSize = componentVoxel->getCubeSize();
		unsigned int chunkSize = componentVoxel->chunkSize;

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

		btScalar totalMass = 0.1f; //Starts out a little above 0 so its not static.
		btVector3 centerOfMass = btVector3(0.0f, 0.0f, 0.0f);

		vector3	tempPos(0, 0, 0);
		unsigned int tempBlockCount = 0;

		for (unsigned int x = 0; x < chunkSize; x++)
		{
			for (unsigned int y = 0; y < chunkSize; y++)
			{
				for (unsigned int z = 0; z < chunkSize; z++)
				{
					tempPos = vector3(x, y, z);
					tempPos *= cubeSize;

					BlockID id = componentVoxel->getBlock(x, y, z);

					if (id == 1)
					{
						tempBlockCount++;
						float blockMass = 1.0f;

						totalMass += blockMass;
						centerOfMass += (btVector3(x, y, z) * blockMass);


						//If surrounded by blocks do nothing
						if (componentVoxel->getBlock(x + 1, y, z) == 1 && componentVoxel->getBlock(x - 1, y, z) == 1 && componentVoxel->getBlock(x, y + 1, z) == 1 && componentVoxel->getBlock(x, y - 1, z) == 1 && componentVoxel->getBlock(x, y, z + 1) == 1 && componentVoxel->getBlock(x, y, z - 1) == 1)
						{
							continue;
						}

						//Top
						if (componentVoxel->getBlock(x, y + 1, z) == 0)
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
						if (componentVoxel->getBlock(x, y - 1, z) == 0)
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
						if (componentVoxel->getBlock(x, y, z + 1) == 0)
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
						if (componentVoxel->getBlock(x, y, z - 1) == 0)
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
						if (componentVoxel->getBlock(x + 1, y, z) == 0)
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
						if (componentVoxel->getBlock(x - 1, y, z) == 0)
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

		//If we have no blocks delete the object.
		if (tempBlockCount == 0)
		{
			return -1;
		}

		//Get the final center of mass;
		centerOfMass /= tempBlockCount;


		if (entity.has_component<MeshComponent>())
		{
			ComponentHandle<MeshComponent> componentMesh = entity.component<MeshComponent>();
			componentMesh->mesh.addVertices(tempVertices, tempColors, tempIndices);
			componentMesh->offset = -vector3(centerOfMass.getX(), centerOfMass.getY(), centerOfMass.getZ());
		}


		//Physics Updates
		if (entity.has_component<RigidBody>())
		{
			ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();

			if (rigidBody->collisionShape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
			{
				btCompoundShape* compoundShape = (btCompoundShape*)rigidBody->collisionShape;

				//Remove all Shapes
				for (int i = 0; i < compoundShape->getNumChildShapes(); i++)
				{
					compoundShape->removeChildShapeByIndex(i);
				}


				//Add all new shapes back
				for (unsigned int x = 0; x < chunkSize; x++)
				{
					for (unsigned int y = 0; y < chunkSize; y++)
					{
						for (unsigned int z = 0; z < chunkSize; z++)
						{
							//delete all old shapes
							if (componentVoxel->collisionChunk[x][y][z] != 0)
							{
								delete componentVoxel->collisionChunk[x][y][z];
								componentVoxel->collisionChunk[x][y][z] = 0;
							}

							//If surrounded by blocks do nothing
							if (componentVoxel->getBlock(x + 1, y, z) == 1 && componentVoxel->getBlock(x - 1, y, z) == 1 && componentVoxel->getBlock(x, y + 1, z) == 1 && componentVoxel->getBlock(x, y - 1, z) == 1 && componentVoxel->getBlock(x, y, z + 1) == 1 && componentVoxel->getBlock(x, y, z - 1) == 1)
							{
								continue;
							}
							
							//Add new block collider
							if (componentVoxel->getBlock(x, y, z) == 1)
							{
								componentVoxel->collisionChunk[x][y][z] = new btBoxShape(btVector3(cubeSize / 2.0f, cubeSize / 2.0f, cubeSize / 2.0f));

								//offset for new center of mass;
								btVector3 pos = (btVector3(x, y, z) - centerOfMass) * cubeSize;
								compoundShape->addChildShape(btTransform(btQuaternion(0, 0, 0, 1), pos), componentVoxel->collisionChunk[x][y][z]);
							}
						}
					}
				}




				//Readjust the mass properties
				btVector3 inertia;
				rigidBody->collisionShape->calculateLocalInertia(totalMass, inertia);
				rigidBody->rigidBody->setMassProps(totalMass, inertia);

				//Shift position to adjust for new center
				if (entity.has_component<Transform>())
				{
					entity.component<Transform>()->position += vector3(centerOfMass.getX(), centerOfMass.getY(), centerOfMass.getZ());
				}

			}
		}


		return 0;
	}


};

#endif //VOXELSYSTEM_HPP