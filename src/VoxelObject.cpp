#include "VoxelObject.hpp"
#include "World.hpp"
#include "PhysicsWorld.hpp"
#include "RigidBodyComponent.hpp"

#include <iostream>

VoxelObject::VoxelObject(unsigned int id):
GameObject(id)
{

    std::vector<vector3> vertices = std::vector<vector3>();
    std::vector<vector3> colors = std::vector<vector3>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();

    for(unsigned int x = 0; x < chunkSize; x++)
    {
            for(unsigned int y = 0; y < chunkSize; y++)
            {
                for(unsigned int z = 0; z < chunkSize; z++)
                {
                    chunk[x][y][z] = 1;
                    if( (x == 0 || x == chunkSize - 1) || (y == 0 || y == chunkSize - 1) || (z == 0 || z == chunkSize - 1) )
                    {
                        chunk[x][y][z] = 1;
                    }
                }
            }
    }

    unsigned int offset = 0;
    for(unsigned int x = 0; x < chunkSize; x++)
    {
            for(unsigned  int y = 0; y < chunkSize; y++)
            {
                for(unsigned int z = 0; z < chunkSize; z++)
                {
                    //TOP
                    if(chunk[x][y][z] == 1 && (chunk[x][y + 1][z] == 0 || y == chunkSize - 1))
                    {
                            std::cout << "X: " << x << " Y: " << y << " Z: " << z << std::endl;
                            vertices.push_back( vector3( 0.5f,  0.5f, -0.5f) + vector3(x, y, z));
                            vertices.push_back( vector3( 0.5f,  0.5f,  0.5f) + vector3(x, y, z));
                            vertices.push_back( vector3(-0.5f,  0.5f,  0.5f) + vector3(x, y, z));
                            vertices.push_back( vector3(-0.5f,  0.5f, -0.5f) + vector3(x, y, z));

                            indices.push_back(0 + offset);
                            indices.push_back(3 + offset);
                            indices.push_back(1 + offset);
                            indices.push_back(3 + offset);
                            indices.push_back(2 + offset);
                            indices.push_back(1 + offset);
                            colors.push_back( vector3(1.0F, 1.0F, 0.0F) );
                            colors.push_back( vector3(0.0F, 1.0F, 1.0F) );
                            colors.push_back( vector3(1.0F, 0.0F, 1.0F) );
                            colors.push_back( vector3(0.0F, 1.0F, 1.0F) );
                            offset += 4;
                    }
                    //Bottom
                    if(chunk[x][y][z] == 1 || y == 0)
                    {
                            std::cout << "X: " << x << " Y: " << y << " Z: " << z << std::endl;
                            vertices.push_back( vector3( 0.5f, -0.5f, -0.5f) + vector3(x, y, z));
                            vertices.push_back( vector3( 0.5f, -0.5f,  0.5f) + vector3(x, y, z));
                            vertices.push_back( vector3(-0.5f, -0.5f,  0.5f) + vector3(x, y, z));
                            vertices.push_back( vector3(-0.5f, -0.5f, -0.5f) + vector3(x, y, z));

                            indices.push_back(0 + offset);
                            indices.push_back(3 + offset);
                            indices.push_back(1 + offset);
                            indices.push_back(3 + offset);
                            indices.push_back(2 + offset);
                            indices.push_back(1 + offset);
                            colors.push_back( vector3(1.0F, 1.0F, 0.0F) );
                            colors.push_back( vector3(0.0F, 1.0F, 1.0F) );
                            colors.push_back( vector3(1.0F, 0.0F, 1.0F) );
                            colors.push_back( vector3(0.0F, 1.0F, 1.0F) );
                            offset += 4;
                    }
                }
            }
    }

    voxelMesh.addVertices(vertices, colors, indices);
}

void VoxelObject::update()
{
    btTransform temp;
    body->getMotionState()->getWorldTransform(temp);
    this->transform = temp;
    GameObject::update();
}

void VoxelObject::render()
{
    GameObject::render();
    voxelMesh.draw();
}

VoxelObject::~VoxelObject()
{

}
