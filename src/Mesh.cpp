#include "Mesh.hpp"


Mesh::Mesh()
{

}

void Mesh::addVertices(const vector3 vertices[], const vector3 colors[], const int indices[], const int sizeOfVertices, const int sizeOfIndices)
{
    size = sizeOfIndices;

    // Generate 1 buffer, put the resulting identifier in vbo
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &cbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, &colors[0], GL_STATIC_DRAW);
}

void Mesh::draw()
{
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );

        glBindBuffer(GL_ARRAY_BUFFER, cbo);
        glVertexAttribPointer(
        1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_QUADS, 0, size);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &cbo);
}
