#ifndef MESH_HPP
#define MESH_HPP

#include "openGL.hpp"
#include "glmInclude.hpp"
#include <vector>

class Mesh
{
    public:
        Mesh();
        ~Mesh();
        void addVertices(const vector3 vertices[], const vector3 colors[], const int indices[], const int sizeOfVertices, const int sizeOfIndices);
        void addVertices(std::vector<vector3>& vertices, std::vector<vector3>& colors, std::vector<unsigned int>& indices);
        void addVertices(std::vector<vector3>& vertices, std::vector<vector3>& colors, std::vector<vector3>& normals, std::vector<unsigned int>& indices);
        void draw();

    private:
        GLuint vbo;
        GLuint cbo;
        GLuint nbo;
        GLuint ibo;
        int size;

};

#endif // Mesh_HPP
