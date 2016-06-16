#ifndef COLOREDMESH_HPP
#define COLOREDMESH_HPP

#include "Rendering/OpenGL_Include.hpp"
#include "GLM_Include.hpp"
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
	virtual ~ColoredMesh();

	virtual void draw(ShaderProgram* program);

private:
	GLuint vbo;
	GLuint ibo;

	int size;

};

#endif //COLOREDMESH_HPP
