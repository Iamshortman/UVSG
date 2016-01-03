#ifndef NONINDEXEDCOLOREDMESH_HPP
#define NONINDEXEDCOLOREDMESH_HPP

#include "openGL.hpp"
#include "glmInclude.hpp"
#include "Mesh.hpp"
#include <vector>

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

	virtual void draw();

private:
	GLuint vbo;
	int size;

};

#endif //NONINDEXEDCOLOREDMESH_HPP
