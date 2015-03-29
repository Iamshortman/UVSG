#include "Mesh.hpp"


Mesh::Mesh()
{
    //We really don't do anything here
}


void Mesh::addVertices(std::vector<vector3>& vertices, std::vector<vector3>& colors, std::vector<unsigned int>& indices)
{
    //Clear the old one incase
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &cbo);
    glDeleteBuffers(1, &ibo);

    size = indices.size();

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &cbo);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vector3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vector3), &colors[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
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
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &cbo);
    glDeleteBuffers(1, &ibo);
}
