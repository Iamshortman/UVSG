#ifndef MESH_HPP
#define MESH_HPP

#include "openGL.hpp"
#include "glmInclude.hpp"
#include <vector>

class Mesh
{
    public:
        virtual void draw() = 0;
};

#endif // Mesh_HPP
