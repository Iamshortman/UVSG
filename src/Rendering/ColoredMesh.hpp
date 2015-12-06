#ifndef COLOREDMESH_HPP
#define COLOREDMESH_HPP

#include "openGL.hpp"
#include "glmInclude.hpp"
#include "Mesh.hpp"
#include <vector>

struct ColoredVertex
{
	vector3F pos;
	vector3F normal;
	vector3F color;
};

class ColoredMesh : public Mesh
{
public:
	ColoredMesh(std::vector<ColoredVertex>& vertices, std::vector<unsigned int>& indices);
	~ColoredMesh();

	virtual void draw();

private:
	GLuint vbo;
	GLuint ibo;

	int size;

};

#endif //COLOREDMESH_HPP