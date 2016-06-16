#ifndef NONINDEXEDCOLOREDMESH_HPP
#define NONINDEXEDCOLOREDMESH_HPP

#include <vector>

#include "Rendering/OpenGL_Include.hpp"
#include "GLM_Include.hpp"
#include "Mesh.hpp"

struct NonIndexedColoredVertex
{
	vector3F pos;
	vector3F normal;
	vector3F color;
};

class NonIndexedColoredMesh : public Mesh
{
public:
	NonIndexedColoredMesh(std::vector<NonIndexedColoredVertex>& vertices);
	virtual ~NonIndexedColoredMesh();

	virtual void draw(ShaderProgram* program);

private:
	GLuint vbo;
	int size;

};

#endif //NONINDEXEDCOLOREDMESH_HPP
