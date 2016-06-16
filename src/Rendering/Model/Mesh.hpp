#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "Rendering/ShaderProgram.hpp"
#include "Rendering/OpenGL_Include.hpp"
#include "GLM_Include.hpp"

class Mesh
{
    public:
		virtual void draw(ShaderProgram* program) = 0;
};

#endif // Mesh_HPP
