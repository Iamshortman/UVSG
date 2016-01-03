#ifndef TEXTUREDMESH_HPP
#define TEXTUREDMESH_HPP

#include "openGL.hpp"
#include "glmInclude.hpp"
#include "Mesh.hpp"
#include <vector>

struct TexturedVertex
{
	vector3F pos;
	vector3F normal;
	vector2F uv;
};

class TexturedMesh : public Mesh
{
public:
	TexturedMesh(std::vector<TexturedVertex>& vertices, std::vector<unsigned int>& indices);
	virtual ~TexturedMesh();

	virtual void draw();

private:
	GLuint vbo;
	GLuint ibo;

	int size;

};

#endif //TEXTUREDMESH_HPP
