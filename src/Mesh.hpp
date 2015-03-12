#ifndef MESH_HPP
#define MESH_HPP

#include "glmInclude.hpp"
#include "openGL.hpp"

class Mesh
{
    public:
        Mesh();
        ~Mesh();
        void addVertices(const vector3 vertices[], const vector3 colors[], const int indices[]);
        void draw();

    private:
        GLuint vbo;
        GLuint ibo;
        GLuint cbo;
        int size;

};

#endif // Mesh_HPP
