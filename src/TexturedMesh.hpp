#ifndef TEXTUREDMESH_HPP
#define TEXTUREDMESH_HPP

#include "openGL.hpp"
#include "glmInclude.hpp"
#include <vector>

struct Vertex
{
	vector3 pos;
	vector3 normal;
	vector2 uv;
};

class TexturedMesh
{
public:
	TexturedMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	~TexturedMesh();

	void draw();

private:
	GLuint vbo;
	GLuint ibo;

	int size;

};

#endif // TEXTUREDMESH_HPP