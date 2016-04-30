#ifndef MATERIALMESH_HPP
#define MATERIALMESH_HPP

#include "openGL.hpp"
#include "glmInclude.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include <vector>

struct MaterialVertex
{
	vector3F pos;
	vector3F normal;
	float material;
};

class MaterialMesh : public Mesh
{
public:
	MaterialMesh(std::vector<MaterialVertex>& vertices, std::vector<Material> materials);
	virtual ~MaterialMesh();

	virtual void draw(ShaderProgram* program);

	std::vector<Material> materials;
private:
	GLuint vbo;
	int size;
};

#endif //MATERIALMESH_HPP