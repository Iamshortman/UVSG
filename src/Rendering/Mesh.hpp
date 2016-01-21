#ifndef MESH_HPP
#define MESH_HPP

#include "ShaderProgram.hpp"
#include "openGL.hpp"
#include <glmInclude.hpp>
#include <vector>

class Mesh
{
    public:
		virtual void draw(ShaderProgram* program) = 0;
};

#endif // Mesh_HPP
