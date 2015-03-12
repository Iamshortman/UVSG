
#include "Mesh.hpp"


Mesh::Mesh()
{

}

void Mesh::addVertices(const vector3 vertices[], const vector3 colors[], const int indices[])
{
        size = sizeof(indices);

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &cbo);
        glGenBuffers(1, &ibo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Mesh::draw()
{

}

Mesh::~Mesh()
{

}
