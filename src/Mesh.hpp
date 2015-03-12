#ifndef MESH_HPP
#define MESH_HPP

#include "openGL.hpp"
#include "glmInclude.hpp"

class Mesh
{
    public:
        Mesh();
        ~Mesh();
        void addVertices(const vector3 vertices[], const vector3 colors[], const int indices[], const int sizeOfVertices);
        void draw();

    private:
        GLuint vbo;
        GLuint cbo;
        GLuint ibo;
        int size;

};

#endif // Mesh_HPP
